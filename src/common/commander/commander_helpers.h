/**
 *
 *   ██████╗   ██╗ ███████╗ ███╗   ███╗ ██╗   ██╗
 *   ██╔══██╗ ███║ ██╔════╝ ████╗ ████║ ██║   ██║
 *   ██████╔╝ ╚██║ █████╗   ██╔████╔██║ ██║   ██║
 *   ██╔══██╗  ██║ ██╔══╝   ██║╚██╔╝██║ ██║   ██║
 *   ██║  ██║  ██║ ███████╗ ██║ ╚═╝ ██║ ╚██████╔╝
 *   ╚═╝  ╚═╝  ╚═╝ ╚══════╝ ╚═╝     ╚═╝  ╚═════╝
 *
 * @file commander_helpers.h
 * @brief CommanderInfo contains all commander helpers functions
 *
 * @license GNU GENERAL PUBLIC LICENSE - Version 2, June 1991
 *          See LICENSE file for further information
 */

#pragma once

#include "R1EMU.h"
#include "commander.h"

#pragma pack(push, 1)
/**
 * @brief CommanderAppearance is the struct of a commander sent to the client
 */
typedef struct CommanderAppearance
{
    uint8_t commanderName[COMMANDER_NAME_SIZE+1];
    uint8_t familyName[COMMANDER_FAMILY_NAME_SIZE];
    uint8_t unk2[7];
    uint64_t accountId;
    uint16_t classId;
    uint16_t unk4;
    uint16_t jobId;
    uint8_t gender;
    uint8_t unk5;
    uint32_t level;
    uint32_t equipment[EQSLOT_Count];
    uint8_t hairId;
    uint8_t unk6;
    uint16_t pose;
} CommanderAppearance;
#pragma pack(pop)

#pragma pack(push, 1)
/**
 * Structure of variables needed for BC_COMMANDER_CREATE
 */
typedef struct CommanderCreatePacket {
    CommanderAppearance appearance;
    uint64_t socialInfoId;
    uint16_t commanderPosition;
    uint16_t mapId;
    uint32_t unk4;
    uint32_t unk5;
    uint32_t maxXP;
    uint32_t unk6;
    PositionXYZ pos;
    PositionXZ dir;
    PositionXYZ pos2;
    PositionXZ dir2;
    uint32_t unk8;
} CommanderCreatePacket;
#pragma pack(pop)

/**
 * @brief Get commander's appearance client structure
 * @param Commander commander
 * @param Appearance An CommanderAppearance structure
 */
void helperCommanderGetCommanderAppearance(Commander *commander, CommanderAppearance *appearance);
