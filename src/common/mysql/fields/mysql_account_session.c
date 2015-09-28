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

#include "mysql_account_session.h"
#include "barrack_server/barrack_handler/barrack_builder.h"
#include "mysql_commander.h"
#include "mysql_item.h"

bool mySqlLoadAccountCommanders(MySQL *self, AccountSession *accountSession, uint64_t accountId, size_t *commandersCount) {

    bool status = false;

    // Request to SQL the list of commanders, retrieve the count
    if (!(mySqlRequestCommandersByAccountId(self, accountId, commandersCount))) {
        error("Cannot request commanders by accountId = %llx", accountId);
        goto cleanup;
    }

    // Allocate the commanders array depending of the count of commanders
    accountSession->commandersCountMax = 5; /// FIXME
    if (!(accountSessionCommandersInit(accountSession, accountSession->commandersCountMax, *commandersCount))) {
        error("Cannot initialize commanders in session.");
        goto cleanup;
    }

    // Write commanders array from SQL result
    if (!(mySqlGetCommanders(self, accountSession->commanders))) {
        error("Cannot get commanders by accountId = %llx", accountId);
        goto cleanup;
    }

    for (int commanderIndex = 0; commanderIndex < *commandersCount; commanderIndex++) {

        // Load items
        size_t itemsCount;
        Commander *commander = accountSession->commanders[commanderIndex];
        Inventory *inventory = &commander->inventory;
        if (!(mySqlRequestItems(self, commander->commanderId, &itemsCount))) {
            error("Cannot request items for commander %d", commanderIndex);
            goto cleanup;
        }
        Item *items = calloc(itemsCount, sizeof(Item));
        if (!(mySqlGetItems(self, &items))) {
            error("Cannot get items for commander %d", commanderIndex);
            goto cleanup;
        }

        for (int i = 0; i < itemsCount; i++) {
            dbg("item.itemId %d", items[i].itemId);
            dbg("item.itemCategory %d", items[i].itemCategory);
            inventoryAddItem(inventory, &items[i]);
        }

        inventoryPrintBag(inventory, INVENTORY_CAT_ARMOR);
        inventoryPrintBag(inventory, INVENTORY_CAT_WEAPON);

        // Load equipment
        uint64_t slotItemIds[EQSLOT_Count];
        if (!(mySqlGetSlotItemIds(self, commander->commanderId, slotItemIds))) {
            error("Cannot get equipment slots for commander %d", commanderIndex);
            goto cleanup;
        }
        // Equip items
        for (int eqSlotIndex = 0; eqSlotIndex < EQSLOT_Count; eqSlotIndex++) {
            dbg("eqslotIndex %d: %d", eqSlotIndex, slotItemIds[eqSlotIndex]);
            if (slotItemIds[eqSlotIndex] > 0) {
                inventoryEquipItem(inventory, slotItemIds[eqSlotIndex], eqSlotIndex);
            }
        }

        inventoryPrintEquipment(inventory);

        inventoryPrintBag(inventory, INVENTORY_CAT_ARMOR);
        inventoryPrintBag(inventory, INVENTORY_CAT_WEAPON);
    }

    status = true;

cleanup:
    return status;
}

bool mySqlGetAccountData(
    MySQL *self,
    char *accountName,
    unsigned char *password,
    AccountSession *accountSession,
    bool *goodCredentials)
{
    bool status = false;
    MYSQL_ROW row;

    char md5Password[33];
    md5BytesToStr(password, md5Password);

    // flush the commander
    if (mySqlQuery(self,
        "SELECT "
            MYSQL_ACCOUNT_SESSION_FIELD_account_id_str ", "
            MYSQL_ACCOUNT_SESSION_FIELD_is_banned_str ", "
            "Unix_Timestamp(" MYSQL_ACCOUNT_SESSION_FIELD_time_banned_str "), "
            MYSQL_ACCOUNT_SESSION_FIELD_credits_str ", "
            MYSQL_ACCOUNT_SESSION_FIELD_privilege_level_str ", "
            MYSQL_ACCOUNT_SESSION_FIELD_family_name_str ", "
            MYSQL_ACCOUNT_SESSION_FIELD_barrack_type_str " "
        "FROM accounts "
        "WHERE " MYSQL_ACCOUNT_SESSION_FIELD_account_name_str " = '%s' "
        "AND " MYSQL_ACCOUNT_SESSION_FIELD_passwd_str " = '%s' "
        "LIMIT 1",
        accountName, md5Password))
    {
        error("SQL Error : %s" , mysql_error(self->handle));
        goto cleanup;
    }

    if (mysql_num_rows(self->result) == 0) {
        // Wrong account / password
        warning("MySQL: Account/Password is incorrect.");
        *goodCredentials = false;
    } else {
        // update the commander
        row = mysql_fetch_row(self->result);
        strncpy(accountSession->accountName, accountName, sizeof(accountSession->accountName));
        accountSession->accountId = strtoll(row[0], NULL, 10);
        accountSession->isBanned = row[1][0] == 'y';
        accountSession->timeBanned = strtol(row[2], NULL, 10); // FIXME : not sure if 32 or 64 bits
        accountSession->credits = strtof(row[3], NULL);
        accountSession->privilege = strtol(row[4], NULL, 10);
        strncpy(accountSession->familyName, row[5], sizeof(accountSession->familyName));
        accountSession->barrackType = strtol(row[6], NULL, 10);
        *goodCredentials = true;
    }

    status = true;

cleanup:
    return status;
}

BarrackNameChangeStatus mySqlSetFamilyName(MySQL *self, uint64_t accountId, char *familyName) {

    BarrackNameChangeStatus status = BC_BARRACKNAME_CHANGE_ERROR;
    MYSQL_ROW row;

    // Perform query to change name
    if (mySqlQuery(self, "CALL bSetFamilyName(%llu, '%s');", accountId, familyName)) {
        error("SQL Error : %s" , mysql_error(self->handle));
        goto cleanup;
    }

    // Check query results
    if (mySqlQuery(self, "SELECT @flag;")) {
        error("SQL Error : %s" , mysql_error(self->handle));
        goto cleanup;
    }

    if (mysql_num_rows(self->result) == 0) {
        error("MySQL: Procedure bSetFamilyName(%llu, '%s') didn't return value", accountId, familyName);
        goto cleanup;
    }

    row = mysql_fetch_row(self->result);
    status = strtol(row[0], NULL, 10);

cleanup:
    return status;
}
