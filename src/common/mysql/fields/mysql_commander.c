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

#include "mysql_commander.h"
#include "common/commander/commander.h"

bool mySqlGetCommanders(MySQL *self, Commander **commanders) {

    MYSQL_ROW row;

    dbg("mySqlGetCommanders count: %d", mysql_num_rows(self->result));

    for (int i = 0; (row = mysql_fetch_row(self->result)); i++) {

        Commander *curCommander = commanders[i];
        commanderInit(curCommander);

        curCommander->mapId = strtol(row[MYSQL_COMMANDER_FIELD_map_id], NULL, 10);
        curCommander->pcId = strtoll(row[MYSQL_COMMANDER_FIELD_commander_id], NULL, 10);
        curCommander->commanderId = strtoll(row[MYSQL_COMMANDER_FIELD_commander_id], NULL, 10);
        curCommander->socialInfoId = strtoll(row[MYSQL_COMMANDER_FIELD_commander_id], NULL, 10);
        strncpy(curCommander->name, row[MYSQL_COMMANDER_FIELD_commanderName], sizeof(curCommander->name));
        curCommander->jobId = strtol(row[MYSQL_COMMANDER_FIELD_job_id], NULL, 10);
        curCommander->classId = strtol(row[MYSQL_COMMANDER_FIELD_class_id], NULL, 10);
        curCommander->hairId = strtol(row[MYSQL_COMMANDER_FIELD_hair_id], NULL, 10);
        curCommander->gender = strtol(row[MYSQL_COMMANDER_FIELD_gender], NULL, 10);
        curCommander->level = strtol(row[MYSQL_COMMANDER_FIELD_level], NULL, 10);
    }

    return true;
}

bool mySqlRequestCommandersByAccountId(MySQL *self, uint64_t accountId, size_t *_commandersCount) {

    bool status = false;
    size_t commandersCount = 0;

    // check if current commander exists
    if (mySqlQuery(self, "SELECT "
        " "  MYSQL_COMMANDER_FIELD_commander_id_str
        ", " MYSQL_COMMANDER_FIELD_commanderName_str
        ", " MYSQL_COMMANDER_FIELD_time_deleted_str
        ", " MYSQL_COMMANDER_FIELD_level_str
        ", " MYSQL_COMMANDER_FIELD_exp_str
        ", " MYSQL_COMMANDER_FIELD_gender_str
        ", " MYSQL_COMMANDER_FIELD_job_id_str
        ", " MYSQL_COMMANDER_FIELD_class_id_str
        ", " MYSQL_COMMANDER_FIELD_hair_id_str
        ", " MYSQL_COMMANDER_FIELD_map_id_str
        ", " MYSQL_COMMANDER_FIELD_position_x_str
        ", " MYSQL_COMMANDER_FIELD_position_y_str
        ", " MYSQL_COMMANDER_FIELD_position_z_str
        ", " MYSQL_COMMANDER_FIELD_hp_str
        ", " MYSQL_COMMANDER_FIELD_mp_str
        " FROM commanders "
        "WHERE account_id = '%llu' "
        "AND is_deleted = 'n' ",
        accountId))
    {
        error("SQL Error : %s" , mysql_error(self->handle));
        goto cleanup;
    }

    commandersCount = mysql_num_rows(self->result);

    dbg("commanders found in account %d", commandersCount);

    *_commandersCount = commandersCount;
    status = true;

cleanup:
    return status;
}

bool mySqlCommanderInsert(MySQL *self, uint64_t accountId, Commander *commanderToCreate) {

    MYSQL_ROW row;
    bool status = false;
    uint32_t commanderId;

    if (mySqlQuery(self,
                   "CALL createCommander(%llu, '%s', %d, %d, %d, %d, %d, %d, %f, %f, %f, %f, %f);"
                   , accountId, commanderToCreate->name, commanderToCreate->level
                   , commanderToCreate->gender, commanderToCreate->jobId, commanderToCreate->classId
                   , commanderToCreate->hairId, commanderToCreate->mapId, commanderToCreate->pos.x
                   , commanderToCreate->pos.y, commanderToCreate->pos.z, commanderToCreate->currentHP
                   , commanderToCreate->currentStamina
                   )) {
        error("SQL Error : %s" , mysql_error(self->handle));
        goto cleanup;
    }

    // Check query results
    if (mySqlQuery(self, "SELECT @flag;")) {
        error("SQL Error : %s" , mysql_error(self->handle));
        goto cleanup;
    }

    if (mysql_num_rows(self->result) == 0) {
        error("MySQL: Procedure createCommander() didn't return value");
        goto cleanup;
    }

    row = mysql_fetch_row(self->result);
    commanderId = strtol(row[0], NULL, 10);

    // CommanderId = -1 == NAME ALREADY EXIST
    // CommanderId = -2 == SQL EXCEPTION
    // CommanderId =  0 == Not used
    // CommanderId > 0 = Commander ID

    if (commanderId < 1) {
        error("There was an error in MySQL creating a commander.");
        goto cleanup;
    }

    commanderToCreate->commanderId = commanderId;

    status = true;

cleanup:
    return status;
}

bool MySqlCommanderDelete(MySQL *self, uint64_t commanderId) {

    bool status = false;

// Insert a new commander
    if (mySqlQuery(self, "DELETE FROM commanders "
       "WHERE commander_id = '%llu'",
       commanderId))
    {
        error("SQL Error : %s" , mysql_error(self->handle));
        goto cleanup;
    }

    status = true;

cleanup:
    return status;
}
