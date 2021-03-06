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

#include "commander.h"

bool commanderEquipmentInit(uint32_t *equipment) {

    equipment[EQSLOT_HEAD_TOP] = 2;
    equipment[EQSLOT_HEAD_MIDDLE] = 2;
    equipment[EQSLOT_UNKOWN1] = 4;
    equipment[EQSLOT_BODY_ARMOR] = 0x81a9d;
    equipment[EQSLOT_GLOVES] = 6;
    equipment[EQSLOT_BOOTS] = 7;
    equipment[EQSLOT_HELMET] = 0x2710;
    equipment[EQSLOT_BRACELET] = 0x2af8;
    equipment[EQSLOT_WEAPON] = 0x3118d;
    equipment[EQSLOT_SHIELD] = 0x98967c;
    equipment[EQSLOT_COSTUME] = 4;
    equipment[EQSLOT_UNKOWN3] = 9;
    equipment[EQSLOT_UNKOWN4] = 9;
    equipment[EQSLOT_UNKOWN5] = 4;
    equipment[EQSLOT_LEG_ARMOR] = 0x7f38d;
    equipment[EQSLOT_UNKOWN6] = 9;
    equipment[EQSLOT_UNKOWN7] = 9;
    equipment[EQSLOT_RING_LEFT] = 9;
    equipment[EQSLOT_RING_RIGHT] = 9;
    equipment[EQSLOT_NECKLACE] = 0xa;

    return true;
}

Commander *commanderNew(void) {
    Commander *self;

    if ((self = calloc(1, sizeof(Commander))) == NULL) {
        error("Cannot allocate a new commander.");
        return NULL;
    }

    if (!commanderInit (self)) {
        commanderDestroy (&self);
        error("Router failed to initialize.");
        return NULL;
    }

    return self;
}

Commander *commanderDup(Commander *src) {
    Commander *dest = NULL;

    if (!(dest = commanderNew())) {
        error("Cannot allocate a new commander.");
        return NULL;
    }

    memcpy(dest, src, sizeof(*src));

    return dest;
}

bool commanderInit(Commander *commander) {
    memset(commander, 0, sizeof(*commander));

    commander->accountId = -1;
    commander->classId = COMMANDER_CLASS_CLERIC;
    commander->unk4 = SWAP_UINT16(0x2220); // ICBT
    commander->jobId = COMMANDER_JOB_CLERIC; // Cleric
    commander->gender = COMMANDER_GENDER_FEMALE; // Female
    commander->unk5 = 0; // ICBT
    commander->level = 1;
    commander->hairId = 0x10;
    commander->pose = SWAP_UINT16(0x0000); // Idle (ICBT)

    commander->pos = PositionXYZ_decl(27.0, 30.0, 29.0);
    commander->currentXP = 0;
    commander->maxXP = 0xC; // ICBT
    commander->pcId = -1;
    commander->socialInfoId = -1;
    commander->commanderId = -1;
    commander->currentHP = 238;
    commander->maxHP = 238;
    commander->currentSP = 105;
    commander->maxSP = 105;
    commander->currentStamina = 25000;
    commander->maxStamina = 25000;

    if (!(inventoryInit(&commander->inventory))) {
        error("Cannot initialize inventory.");
        return false;
    }

    return true;
}

void commanderEquipmentPrint(uint32_t *equipment) {
    /*
    dbg("head_top = %d (%x)", equipment->head_top, equipment->head_top);
    dbg("head_middle = %d (%x)", equipment->head_middle, equipment->head_middle);
    dbg("itemUnk1 = %d (%x)", equipment->itemUnk1, equipment->itemUnk1);
    dbg("body_armor = %d (%x)", equipment->body_armor, equipment->body_armor);
    dbg("gloves = %d (%x)", equipment->gloves, equipment->gloves);
    dbg("boots = %d (%x)", equipment->boots, equipment->boots);
    dbg("helmet = %d (%x)", equipment->helmet, equipment->helmet);
    dbg("bracelet = %d (%x)", equipment->bracelet, equipment->bracelet);
    dbg("weapon = %d (%x)", equipment->weapon, equipment->weapon);
    dbg("shield = %d (%x)", equipment->shield, equipment->shield);
    dbg("costume = %d (%x)", equipment->costume, equipment->costume);
    dbg("itemUnk3 = %d (%x)", equipment->itemUnk3, equipment->itemUnk3);
    dbg("itemUnk4 = %d (%x)", equipment->itemUnk4, equipment->itemUnk4);
    dbg("itemUnk5 = %d (%x)", equipment->itemUnk5, equipment->itemUnk5);
    dbg("leg_armor = %d (%x)", equipment->leg_armor, equipment->leg_armor);
    dbg("itemUnk6 = %d (%x)", equipment->itemUnk6, equipment->itemUnk6);
    dbg("itemUnk7 = %d (%x)", equipment->itemUnk7, equipment->itemUnk7);
    dbg("ring_left = %d (%x)", equipment->ring_left, equipment->ring_left);
    dbg("ring_right = %d (%x)", equipment->ring_right, equipment->ring_right);
    dbg("necklace = %d (%x)", equipment->necklace, equipment->necklace);
    */
}

void commanderPrint(Commander *commander) {
    dbg("commanderName = %s", commander->name);
    dbg("familyName = %s", commander->familyName);
    dbg("accountId = %llu (%llx)", commander->accountId, commander->accountId);
    dbg("classId = %d (%x)", commander->classId, commander->classId);
    dbg("unk4 = %d (%x)", commander->unk4, commander->unk4);
    dbg("jobId = %d (%x)", commander->jobId, commander->jobId);
    dbg("gender = %d (%x)", commander->gender, commander->gender);
    dbg("unk5 = %d (%x)", commander->unk5, commander->unk5);
    dbg("level = %d (%x)", commander->level, commander->level);
    dbg("hairId = %d (%x)", commander->hairId, commander->hairId);
    dbg("pose = %d (%x)", commander->pose, commander->pose);
    dbg("posX = %f %f %f (%x %x %x)",
         commander->pos.x, commander->pos.y, commander->pos.z,
         commander->pos.x, commander->pos.y, commander->pos.z);
    dbg("currentXP = %d (%x)", commander->currentXP, commander->currentXP);
    dbg("maxXP = %d (%x)", commander->maxXP, commander->maxXP);
    dbg("socialInfoId = %llu (%llx)", commander->socialInfoId, commander->socialInfoId);
    dbg("commanderId = %llu (%llx)", commander->commanderId, commander->commanderId);
    dbg("pcId = %llu (%llx)", commander->pcId, commander->pcId);
    dbg("currentHP = %d (%x)", commander->currentHP, commander->currentHP);
    dbg("maxHP = %d (%x)", commander->maxHP, commander->maxHP);
    dbg("currentSP = %d (%x)", commander->currentSP, commander->currentSP);
    dbg("maxSP = %d (%x)", commander->maxSP, commander->maxSP);
    dbg("currentStamina = %d (%x)", commander->currentStamina, commander->currentStamina);
    dbg("maxStamina = %d (%x)", commander->maxStamina, commander->maxStamina);
}

void commanderFree(Commander *self) {
    // TODO : free inventory?
}

void commanderDestroy(Commander **_self) {
    Commander *self = *_self;

    if (_self && self) {
        commanderFree(self);
        free(self);
        *_self = NULL;
    }
}

