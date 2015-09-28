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

#include "commander_helpers.h"
#include "commander.h"

void helperCommanderGetCommanderAppearance(Commander *commander, CommanderAppearance *appearance) {
    memcpy(&appearance->commanderName, &commander->name, sizeof(appearance->commanderName));
    memcpy(&appearance->familyName, &commander->familyName, sizeof(appearance->familyName));
    memcpy(&appearance->unk2, &commander->unk2, sizeof(appearance->unk2));
    appearance->accountId = commander->accountId;
    appearance->classId = commander->classId;
    appearance->unk4 = commander->unk4;
    appearance->jobId = commander->jobId;
    appearance->gender = commander->gender;
    appearance->unk5 = commander->unk5;
    appearance->level = commander->level;
    appearance->hairId = commander->hairId;
    appearance->pose = commander->pose;

    Inventory *inventory = &commander->inventory;

    uint32_t itemType;
    for (int i = 0; i < EQSLOT_Count; i++) {
        if (inventory->equippedItems[i]) {
            itemType = inventory->equippedItems[i]->itemType;
        } else {
            inventoryGetEquipmentEmptySlot(i, &itemType);
        }
        appearance->equipment[i] = itemType;
    }
}
