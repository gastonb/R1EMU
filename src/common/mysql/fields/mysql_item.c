/**
*
*   ██████╗   ██╗ ███████╗ ███╗   ███╗ ██╗   ██╗
*   ██╔══██╗ ███║ ██╔════╝ ████╗ ████║ ██║   ██║
*   ██████╔╝ ╚██║ █████╗   ██╔████╔██║ ██║   ██║
*   ██╔══██╗  ██║ ██╔══╝   ██║╚██╔╝██║ ██║   ██║
*   ██║  ██║  ██║ ███████╗ ██║ ╚═╝ ██║ ╚██████╔╝
*   ╚═╝  ╚═╝  ╚═╝ ╚══════╝ ╚═╝     ╚═╝  ╚═════╝
*
* @license GNU GENERAL PUBLIC LICENSE - Version 2, June 1991
 *          See LICENSE file for further information
*/

#include "mysql_item.h"
#include "common/item/item.h"

bool mySqlGetItems(MySQL *self, Item **_items) {

    MYSQL_ROW row;

    Item *items = *_items;

    dbg("mySqlGetItems count: %d", mysql_num_rows(self->result));

    uint64_t itemId;
    uint32_t itemType;
    uint32_t amount;
    uint8_t inventoryIndex;
    for (int i = 0; (row = mysql_fetch_row(self->result)); i++) {
        dbg("preparing item %d", i);
        itemId = strtoull(row[MYSQL_ITEM_FIELD_item_id], NULL, 10);
        itemType = strtoul(row[MYSQL_ITEM_FIELD_item_type], NULL, 10);
        amount = strtoul(row[MYSQL_ITEM_FIELD_amount], NULL, 10);
        inventoryIndex = strtoul(row[MYSQL_ITEM_FIELD_position_in_inventory], NULL, 10);


        Item *item = itemNew(itemId, itemType, amount, inventoryIndex);


        dbg("item %d initialized", i);

        switch (itemId) {
            case 1: {
                item->itemCategory = INVENTORY_CAT_ARMOR;
                break;
            }
            case 2: {
                item->itemCategory = INVENTORY_CAT_WEAPON;
                break;
            }
            case 3: {
                item->itemCategory = INVENTORY_CAT_ARMOR;
                break;
            }
        }

        items[i] = *item;

    }



    return true;
}

bool mySqlRequestItems(MySQL *self, uint64_t commanderId, size_t *_itemsCount) {

    bool status = false;
    size_t itemsCount = 0;

    // check if current commander exists
    if (mySqlQuery(self, "SELECT "
        " "  MYSQL_ITEM_FIELD_item_id_str
        ", " MYSQL_ITEM_FIELD_item_type_str
        ", " MYSQL_ITEM_FIELD_amount_str
        ", " MYSQL_ITEM_FIELD_position_in_inventory_str
        " FROM items "
        "WHERE commander_id = %llx "
        "ORDER BY position_in_inventory ASC ",
        commanderId))
    {
        error("SQL Error : %s" , mysql_error(self->handle));
        goto cleanup;
    }

    itemsCount = mysql_num_rows(self->result);

    dbg("items found in commander %d", itemsCount);

    *_itemsCount = itemsCount;
    status = true;

cleanup:
    return status;
}

bool mySqlGetSlotItemIds(MySQL *self, uint64_t commanderId, uint64_t *slotItemIds) {
    bool status = false;
    MYSQL_ROW row;


    dbg("yoquese");

    dbg("slotItemIds[0] %d", slotItemIds[0]);
    dbg("slotItemIds[1] %d", slotItemIds[1]);

    // check if current commander exists
    if (mySqlQuery(self, "SELECT "
        " " MYSQL_SLOT_FIELD_eqslot_head_top_str
        ", " MYSQL_SLOT_FIELD_eqslot_head_middle_str
        ", " MYSQL_SLOT_FIELD_eqslot_unkown_1_str
        ", " MYSQL_SLOT_FIELD_eqslot_body_armor_str
        ", " MYSQL_SLOT_FIELD_eqslot_gloves_str
        ", " MYSQL_SLOT_FIELD_eqslot_boots_str
        ", " MYSQL_SLOT_FIELD_eqslot_helmet_str
        ", " MYSQL_SLOT_FIELD_eqslot_bracelet_str
        ", " MYSQL_SLOT_FIELD_eqslot_weapon_str
        ", " MYSQL_SLOT_FIELD_eqslot_shield_str
        ", " MYSQL_SLOT_FIELD_eqslot_costume_str
        ", " MYSQL_SLOT_FIELD_eqslot_unkown_3_str
        ", " MYSQL_SLOT_FIELD_eqslot_unkown_4_str
        ", " MYSQL_SLOT_FIELD_eqslot_unkown_5_str
        ", " MYSQL_SLOT_FIELD_eqslot_leg_armor_str
        ", " MYSQL_SLOT_FIELD_eqslot_unkown_6_str
        ", " MYSQL_SLOT_FIELD_eqslot_unkown_7_str
        ", " MYSQL_SLOT_FIELD_eqslot_ring_left_str
        ", " MYSQL_SLOT_FIELD_eqslot_ring_right_str
        ", " MYSQL_SLOT_FIELD_eqslot_necklace_str
        " FROM commander_slots "
        "WHERE commander_id = %llx "
        "LIMIT 1 ",
        commanderId))
    {
        error("SQL Error : %s" , mysql_error(self->handle));
        goto cleanup;
    }

    if (mysql_num_rows(self->result) != 1) {
        error("No data found in commander_slots for commanderId: %d", commanderId);
        goto cleanup;
    }

    row = mysql_fetch_row(self->result);

    for (int eqSlotIndex; eqSlotIndex < EQSLOT_Count; eqSlotIndex++) {
        slotItemIds[eqSlotIndex] = strtoull(row[eqSlotIndex], NULL, 10);
    }

    status = true;

cleanup:
    return status;
}
