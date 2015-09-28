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


// ---------- Includes ------------
#include "mysql_commander_session.h"

bool mySqlCommanderInsertCommander(MySQL *self, Commander *commander) {

    bool status = false;

    // insert a new Commander
    if (mySqlQuery (self,
        "INSERT INTO commander ("
        "`commander_id`, `account_id`, `name`, `family_name`, `class`, "
        "`gender`, `level`, `head_top`, `head_middle`, `necklace`, `body_armor`, `leg_armor`, "
        "`gloves`, `shoes`, `weapon`, `shield`, `costume`, `ring`, `bracelet_left`, "
        "`bracelet_right`, `hair_type`, `last_x`, `last_y`, `last_z`, `current_xp`, "
        "`current_hp`, `current_sp`) VALUES ("
        "%u, %llu, '%s', '%s', %d, "
        "%d, %d, %d, %f, %f, %f, "
        "%d, %d, %d)",
        commander->commanderId,
        commander->accountId,
        commander->name,
        commander->familyName,
        commander->classId,
        commander->gender,
        commander->level,
        commander->hairId,
        commander->pos.x,
        commander->pos.y,
        commander->pos.z,
        commander->currentXP,
        commander->currentHP,
        commander->currentSP))
    {
        error("SQL Error : %s" , mysql_error(self->handle));
        goto cleanup;
    }

    status = true;

cleanup:
    return status;
}

bool mySqlCommanderSessionFlush(MySQL *self, CommanderSession *commanderSession) {

    bool status = false;
    MYSQL_ROW count;

    Commander *commander = commanderSession->currentCommander;

    // flush the commander
    if (mySqlQuery(self, "SELECT count(*) FROM commander WHERE commander_id = %u", commander->commanderId)) {
        error("SQL Error : %s" , mysql_error(self->handle));
        goto cleanup;
    }

    count = mysql_fetch_row(self->result);

    if (atoi(count[0]) == 0) {
        // insert the commander
        if (!(mySqlCommanderInsertCommander(self, commander))) {
            error("Cannot insert a new commander.");
            goto cleanup;
        }
    }
    else {
        // update the commander
        // TODO
    }

    status = true;
cleanup:

    return status;
}
