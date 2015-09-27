/**
 *
 *   ██████╗   ██╗ ███████╗ ███╗   ███╗ ██╗   ██╗
 *   ██╔══██╗ ███║ ██╔════╝ ████╗ ████║ ██║   ██║
 *   ██████╔╝ ╚██║ █████╗   ██╔████╔██║ ██║   ██║
 *   ██╔══██╗  ██║ ██╔══╝   ██║╚██╔╝██║ ██║   ██║
 *   ██║  ██║  ██║ ███████╗ ██║ ╚═╝ ██║ ╚██████╔╝
 *   ╚═╝  ╚═╝  ╚═╝ ╚══════╝ ╚═╝     ╚═╝  ╚═════╝
 *
 * @file mysql_commander.h
 * @brief Mysql fields for commander structure
 *
 * @license GNU GENERAL PUBLIC LICENSE - Version 2, June 1991
 *          See LICENSE file for further information
 */

#pragma once

#include "mysql_session.h"

#define MYSQL_ITEM_FIELD_item_id_str "item_id"
#define MYSQL_ITEM_FIELD_item_type_str "item_type"
#define MYSQL_ITEM_FIELD_amount_str "amount"
#define MYSQL_ITEM_FIELD_position_in_inventory_str "position_in_inventory"

#define MYSQL_SLOT_FIELD_eqslot_head_top_str "eqslot_head_top"
#define MYSQL_SLOT_FIELD_eqslot_head_middle_str "eqslot_head_middle"
#define MYSQL_SLOT_FIELD_eqslot_unkown_1_str "eqslot_unkown_1"
#define MYSQL_SLOT_FIELD_eqslot_body_armor_str "eqslot_body_armor"
#define MYSQL_SLOT_FIELD_eqslot_gloves_str "eqslot_gloves"
#define MYSQL_SLOT_FIELD_eqslot_boots_str "eqslot_boots"
#define MYSQL_SLOT_FIELD_eqslot_helmet_str "eqslot_helmet"
#define MYSQL_SLOT_FIELD_eqslot_bracelet_str "eqslot_bracelet"
#define MYSQL_SLOT_FIELD_eqslot_weapon_str "eqslot_weapon"
#define MYSQL_SLOT_FIELD_eqslot_shield_str "eqslot_shield"
#define MYSQL_SLOT_FIELD_eqslot_costume_str "eqslot_costume"
#define MYSQL_SLOT_FIELD_eqslot_unkown_3_str "eqslot_unkown_3"
#define MYSQL_SLOT_FIELD_eqslot_unkown_4_str "eqslot_unkown_4"
#define MYSQL_SLOT_FIELD_eqslot_unkown_5_str "eqslot_unkown_5"
#define MYSQL_SLOT_FIELD_eqslot_leg_armor_str "eqslot_leg_armor"
#define MYSQL_SLOT_FIELD_eqslot_unkown_6_str "eqslot_unkown_6"
#define MYSQL_SLOT_FIELD_eqslot_unkown_7_str "eqslot_unkown_7"
#define MYSQL_SLOT_FIELD_eqslot_ring_left_str "eqslot_ring_left"
#define MYSQL_SLOT_FIELD_eqslot_ring_right_str "eqslot_ring_right"
#define MYSQL_SLOT_FIELD_eqslot_necklace_str "eqslot_necklace"

enum MySqlItemFields {
    MYSQL_ITEM_FIELD_item_id,
	MYSQL_ITEM_FIELD_item_type,
	MYSQL_ITEM_FIELD_amount,
	MYSQL_ITEM_FIELD_position_in_inventory,
} MySqlItemFields;

bool mySqlGetItems(MySQL *self, Item **items);
bool mySqlRequestItems(MySQL *self, uint64_t commanderId, size_t *_itemsCount);
bool mySqlGetSlotItemIds(MySQL *self, uint64_t commanderId, uint64_t *slotItemIds);
