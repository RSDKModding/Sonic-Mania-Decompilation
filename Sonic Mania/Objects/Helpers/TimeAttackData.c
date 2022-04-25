// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TimeAttackData Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectTimeAttackData *TimeAttackData;

void TimeAttackData_Update(void) {}

void TimeAttackData_LateUpdate(void) {}

void TimeAttackData_StaticUpdate(void) {}

void TimeAttackData_Draw(void) {}

void TimeAttackData_Create(void *data) {}

void TimeAttackData_StageLoad(void) {}

#if RETRO_USE_PLUS
void TimeAttackData_TrackActClear(StatInfo *stat, uint8 zone, uint8 act, uint8 characterID, int32 time, int32 rings, int32 score)
{
    stat->statID = 0;
    stat->name   = "ACT_CLEAR";
    memset(stat->data, 0, sizeof(stat->data));

    stat->data[0] = (void *)ZoneNames[zone];
    stat->data[1] = (void *)ActNames[act];
    stat->data[2] = (void *)PlayerNames[characterID];
    stat->data[3] = 0;
    stat->data[4] = intToVoid(time);
    stat->data[5] = intToVoid(rings);
    stat->data[6] = intToVoid(score);
}
void TimeAttackData_TrackTAClear(StatInfo *stat, uint8 zone, uint8 actID, uint8 characterID, int32 gameMode, int32 time)
{
    stat->statID = 1;
    stat->name   = "TA_CLEAR";
    memset(stat->data, 0, sizeof(stat->data));

    stat->data[0] = (void *)ZoneNames[zone];
    stat->data[1] = (void *)ActNames[actID];
    stat->data[2] = (void *)PlayerNames[characterID];
    stat->data[3] = (void *)ModeNames[gameMode];
    stat->data[4] = intToVoid(time);
}
void TimeAttackData_TrackEnemyDefeat(StatInfo *stat, uint8 zoneID, uint8 actID, uint8 characterID, bool32 encore, int32 x, int32 y)
{
    stat->statID = 2;
    stat->name   = "ENEMY_DEFEAT";
    memset(stat->data, 0, sizeof(stat->data));

    stat->data[0] = (void *)ZoneNames[zoneID];
    stat->data[1] = (void *)ActNames[actID];
    stat->data[2] = (void *)PlayerNames[characterID];
    stat->data[3] = intToVoid(encore);
    stat->data[4] = intToVoid(x);
    stat->data[5] = intToVoid(y);
}
#endif

void TimeAttackData_Clear(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    param->puyoSelection   = PUYO_SELECTION_NONE;
    memset(param->menuTag, 0, sizeof(param->menuTag));
    param->menuSelection    = 0;
    param->startedTAAttempt = false;
    param->inTimeAttack     = false;
    param->zoneID           = 0;
    param->actID            = 0;

#if RETRO_USE_PLUS
    param->timeAttackRank = 0;
    globals->gameMode     = MODE_MANIA;
#else
    param->timeScore  = 0;
    globals->gameMode = MODE_NOSAVE;
#endif

    globals->suppressAutoMusic = false;
    globals->suppressTitlecard = false;
}
int32 TimeAttackData_GetManiaListPos(int32 zoneID, int32 act, int32 characterID)
{
    int32 listPos = 0;
    switch (zoneID) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5: listPos = act + 2 * zoneID; break;

        case 6: listPos = act + (2 * zoneID + 1); break;

        case 7:
            if (act)
                listPos = 2 * zoneID + 3;
            else
                listPos = (characterID == 3) + 1 + 2 * zoneID;
            break;

        case 8:
        case 9: listPos = act + 2 * (zoneID + 1); break;

        case 10:
        case 11: listPos = act + (2 * zoneID + 3); break;

        default: break;
    }

    LogHelpers_Print("playerID = %d, zoneID = %d, act = %d", characterID, zoneID, act);
    LogHelpers_Print("listPos = %d", listPos);

    return listPos;
}
int32 TimeAttackData_GetEncoreListPos(int32 zoneID, int32 act, int32 characterID)
{
    int32 listPos = 0;
    switch (zoneID) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5: listPos = act + 2 * zoneID; break;

        case 6:
        case 7:
        case 8:
        case 9: listPos = act + 2 * zoneID + 1; break;

        case 10:
        case 11: listPos = act + 2 * (zoneID + 1); break;

        default: break;
    }

    LogHelpers_Print("playerID = %d, zoneID = %d, act = %d", characterID, zoneID, act);
    LogHelpers_Print("listPos = %d", listPos);
    return listPos;
}

void TimeAttackData_GetUnpackedTime(int32 time, int32 *minsPtr, int32 *secsPtr, int32 *millisecsPtr)
{
    int32 m  = time / 6000;
    int32 s  = time % 6000 / 100;
    int32 ms = time % 100;

    if (minsPtr)
        *minsPtr = m;
    if (secsPtr)
        *secsPtr = s;
    if (millisecsPtr)
        *millisecsPtr = ms;
}

uint16 *TimeAttackData_GetRecordedTime(uint8 zoneID, uint8 act, uint8 characterID, uint8 rank)
{
    rank--;
    if (rank >= 3)
        return NULL;

    uint16 *saveRAM = NULL;
    if (globals->saveLoaded == STATUS_OK)
        saveRAM = (uint16 *)&globals->saveRAM[0x800];
    else
        return NULL;

    // 72 = (12 * 6)
    int32 pos = 72 * (characterID - 1) + 6 * zoneID + act * 3 + rank;
    // not sure why theres an offset of 43 uint16's (86 bytes) but whatever ig
    pos += 43;
    return &saveRAM[pos];
}

#if RETRO_USE_PLUS
uint16 TimeAttackData_LoadTimeAttackDB(void (*callback)(bool32 success))
{
    LogHelpers_Print("Loading Time Attack DB");
    globals->taTableLoaded = STATUS_CONTINUE;

    TimeAttackData->loadEntityPtr = SceneInfo->entity;
    TimeAttackData->loadCallback  = callback;
    globals->taTableID            = API.LoadUserDB("TimeAttackDB.bin", TimeAttackData_LoadTimeAttackDB_CB);

    if (globals->taTableID == -1) {
        LogHelpers_Print("Couldn't claim a slot for loading %s", "TimeAttackDB.bin");
        globals->taTableLoaded = STATUS_ERROR;
    }

    return globals->taTableID;
}

void TimeAttackData_LoadTimeAttackDB_CB(int32 statusCode)
{
    if (statusCode == STATUS_OK) {
        globals->taTableLoaded = STATUS_OK;
        API.SetupUserDBRowSorting(globals->taTableID);
        LogHelpers_Print("Load Succeeded! Replay count: %d", API.GetSortedUserDBRowCount(globals->taTableID));
    }
    else {
        LogHelpers_Print("Load Failed! Creating new Time Attack DB");
        TimeAttackData_ResetTimeAttackDB();
    }

    LogHelpers_Print("Replay DB Slot => %d, Load Status => %d", globals->taTableID, globals->taTableLoaded);

    if (TimeAttackData->loadCallback) {
        Entity *entStore = SceneInfo->entity;
        if (TimeAttackData->loadEntityPtr)
            SceneInfo->entity = TimeAttackData->loadEntityPtr;
        TimeAttackData->loadCallback(statusCode == STATUS_OK);
        SceneInfo->entity = entStore;

        TimeAttackData->loadCallback  = NULL;
        TimeAttackData->loadEntityPtr = NULL;
    }
}

void TimeAttackData_ResetTimeAttackDB(void)
{
    uint16 id = API.InitUserDB("TimeAttackDB.bin", DBVAR_UINT8, "zoneID", DBVAR_UINT8, "act", DBVAR_UINT8, "characterID", DBVAR_UINT8, "encore",
                               DBVAR_UINT32, "score", DBVAR_UINT32, "replayID", NULL);
    globals->taTableID = id;
    if (id == (uint16)-1) {
        globals->taTableLoaded = STATUS_ERROR;
    }
    else {
        globals->taTableLoaded = STATUS_OK;
        if (!checkNoSave && globals->saveLoaded == STATUS_OK) {
            TimeAttackData_MigrateLegacySaves();
        }
    }
}

void TimeAttackData_MigrateLegacySaves(void)
{
    if (globals->saveLoaded == STATUS_OK) {
        TimeAttackData->isMigratingData = true;

        LogHelpers_Print("===========================");
        LogHelpers_Print("Migrating Legacy TA Data...");
        LogHelpers_Print("===========================");

        for (int32 zone = 0; zone < 12; ++zone) {
            for (int32 act = 0; act < 2; ++act) {
                for (int32 charID = 0; charID < 3; ++charID) {
                    for (int32 rank = 0; rank < 3; ++rank) {
                        uint16 *records = TimeAttackData_GetRecordedTime(zone, act, charID + 1, rank);
                        if (records && *records) {
                            int32 score = *records;
                            LogHelpers_Print("Import: zone=%d act=%d charID=%d rank=%d -> %d", zone, act, charID + 1, rank, score);
                            TimeAttackData_AddTADBEntry(zone, act, charID + 1, MODE_MANIA, score, NULL);
                        }
                    }
                }
            }
        }

        TimeAttackData->isMigratingData = false;
    }
}

int32 TimeAttackData_AddTimeAttackDBEntry(uint8 zoneID, uint8 act, uint8 characterID, uint8 mode, int32 score)
{
    if (globals->taTableLoaded != STATUS_OK)
        return -1;

    uint16 rowID = API.AddUserDBRow(globals->taTableID);
    int32 encore = mode & 1;

    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT8, "zoneID", &zoneID);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT8, "act", &act);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT8, "characterID", &characterID);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT8, "encore", &encore);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT32, "score", &score);
    API.SetUserDBValue(globals->taTableID, rowID, DBVAR_UINT32, "replayID", NULL);

    uint32 uuid = API.GetUserDBRowUUID(globals->taTableID, rowID);
    char buf[0x20];
    memset(buf, 0, 0x20 * sizeof(char));
    API.GetUserDBRowCreationTime(globals->taTableID, rowID, buf, 23, "%Y/%m/%d %H:%M:%S");
    LogHelpers_Print("Time Attack DB Added Entry");
    LogHelpers_Print("Created at %s", buf);
    LogHelpers_Print("Row ID: %d", rowID);
    LogHelpers_Print("UUID: %08X", uuid);

    return rowID;
}

int32 TimeAttackData_AddTADBEntry(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 score, void (*callback)(bool32 success))
{
    uint16 row  = TimeAttackData_AddTimeAttackDBEntry(zoneID, act, characterID, encore, score);
    uint32 uuid = API.GetUserDBRowUUID(globals->taTableID, row);

    TimeAttackData_ConfigureTableView(zoneID, act, characterID, encore);

    if (API.GetSortedUserDBRowCount(globals->taTableID) > 3) {
        int32 row = API.GetSortedUserDBRowID(globals->taTableID, 3);
        API.RemoveDBRow(globals->taTableID, row);

        TimeAttackData_ConfigureTableView(zoneID, act, characterID, encore);
    }

    int32 c     = 0;
    int32 rowID = 0;
    for (c = 0; c < 3; ++c) {
        rowID = API.GetSortedUserDBRowID(globals->taTableID, c);
        if (API.GetUserDBRowUUID(globals->taTableID, rowID) == uuid)
            break;
    }

    if (c == 3) {
        if (callback)
            callback(false);

        return 0;
    }

    TimeAttackData->uuid         = uuid;
    TimeAttackData->rowID        = rowID;
    TimeAttackData->personalRank = c + 1;

    if (TimeAttackData->isMigratingData) {
        if (callback)
            callback(true);
    }
    else {
        TimeAttackData_SaveTimeAttackDB(callback);
    }

    return c + 1;
}

void TimeAttackData_SaveTimeAttackDB(void (*callback)(bool32 success))
{
    if (API.GetNoSave() || globals->taTableID == (uint16)-1 || globals->taTableLoaded != STATUS_OK) {
        if (callback)
            callback(false);
    }
    else {
        LogHelpers_Print("Saving Time Attack DB");

        TimeAttackData->saveEntityPtr = SceneInfo->entity;
        TimeAttackData->saveCallback  = callback;

        API.SaveUserDB(globals->taTableID, TimeAttackData_SaveTimeAttackDB_CB);
    }
}

void TimeAttackData_SaveTimeAttackDB_CB(int32 statusCode)
{
    if (TimeAttackData->saveCallback) {
        Entity *entStore = SceneInfo->entity;
        if (TimeAttackData->saveEntityPtr)
            SceneInfo->entity = TimeAttackData->saveEntityPtr;
        TimeAttackData->saveCallback(statusCode == STATUS_OK);
        SceneInfo->entity = entStore;

        TimeAttackData->saveCallback  = NULL;
        TimeAttackData->saveEntityPtr = NULL;
    }
}

int32 TimeAttackData_GetScore(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 rank)
{
    if (rank > 3 && rank)
        return 0;

    uint8 rankID = rank - 1;
    if (!TimeAttackData->loaded || characterID != TimeAttackData->characterID || zoneID != TimeAttackData->zoneID || act != TimeAttackData->act
        || encore != TimeAttackData->encore) {
        TimeAttackData_ConfigureTableView(zoneID, act, characterID, encore);
    }

    int32 row = API.GetSortedUserDBRowID(globals->taTableID, rankID);
    if (row == -1)
        return 0;

    int32 score = 0;
    API.GetUserDBValue(globals->taTableID, row, DBVAR_UINT32, "score", &score);

    return score;
}

int32 TimeAttackData_GetReplayID(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 rank)
{
    if (rank > 3 && rank)
        return 0;

    uint8 rankID = rank - 1;
    if (!TimeAttackData->loaded || characterID != TimeAttackData->characterID || zoneID != TimeAttackData->zoneID || act != TimeAttackData->act
        || encore != TimeAttackData->encore) {
        TimeAttackData_ConfigureTableView(zoneID, act, characterID, encore);
    }

    int32 row = API.GetSortedUserDBRowID(globals->taTableID, rankID);
    if (row != -1) {
        int32 replayID = 0;
        API.GetUserDBValue(globals->taTableID, row, DBVAR_UINT32, "replayID", &replayID);

        return replayID;
    }

    return 0;
}

void TimeAttackData_ConfigureTableView(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore)
{
    LogHelpers_Print("ConfigureTableView(%d, %d, %d, %d)", characterID, zoneID, act, encore);

    // setup every sort row ID for every entry
    API.SetupUserDBRowSorting(globals->taTableID);

    // remove any sort row IDs that dont match the following values
    API.AddRowSortFilter(globals->taTableID, DBVAR_UINT8, "zoneID", &zoneID);
    API.AddRowSortFilter(globals->taTableID, DBVAR_UINT8, "act", &act);
    API.AddRowSortFilter(globals->taTableID, DBVAR_UINT8, "characterID", &characterID);
    API.AddRowSortFilter(globals->taTableID, DBVAR_UINT8, "encore", &encore);

    // sort the remaining rows
    API.SortDBRows(globals->taTableID, DBVAR_UINT32, "score", false);

    TimeAttackData->loaded      = true;
    TimeAttackData->zoneID      = zoneID;
    TimeAttackData->act         = act;
    TimeAttackData->characterID = characterID;
    TimeAttackData->encore      = encore & 1;
}

void TimeAttackData_GetLeaderboardRank_CB(bool32 success, int32 rank)
{
    if (success) {
        LogHelpers_Print("Got back leaderboard rank: %d. Not bad!", rank);
        TimeAttackData->leaderboardRank = rank;
    }
}

void TimeAttackData_AddLeaderboardEntry(uint8 zoneID, uint8 act, uint8 characterID, bool32 encore, int32 score)
{
    StatInfo stat;
    TimeAttackData_TrackTAClear(&stat, zoneID, act, characterID, encore, score);
    API.TryTrackStat(&stat);

    LeaderboardID *leaderboardInfo = NULL;
    if (zoneID < 12 && act < 2 && characterID <= 5) {
        int32 id = 10 * zoneID + 5 * act + (characterID - 1);
        if (encore)
            id += 120;

        leaderboardInfo = &maniaLeaderboardInfo[id];
    }

    API.TrackScore(leaderboardInfo, score, TimeAttackData_GetLeaderboardRank_CB);
}

#else
void TimeAttackData_SaveTATime(uint8 zone, uint8 act, uint8 characterID, uint8 rank, uint16 score)
{
    rank--;
    if (rank < 3) {
        // playerID * (12 zones)
        // zone * (2 acts)
        // act * (3 ranks)
        uint16 *record = TimeAttackData_GetRecordedTime(zone, act, characterID, 1);

        for (int32 r = 2; r > rank; --r) record[r] = record[r - 1];

        record[rank] = score;
    }
}
#endif

#if RETRO_INCLUDE_EDITOR
void TimeAttackData_EditorDraw(void) {}

void TimeAttackData_EditorLoad(void) {}
#endif

void TimeAttackData_Serialize(void) {}
