#include "SonicMania.h"

ObjectTimeAttackData *TimeAttackData;

void TimeAttackData_Update(void) {}
void TimeAttackData_LateUpdate(void) {}
void TimeAttackData_StaticUpdate(void) {}
void TimeAttackData_Draw(void) {}
void TimeAttackData_Create(void *data) {}
void TimeAttackData_StageLoad(void) {}

void TimeAttackData_TrackActClear(byte act, byte zone, StatInfo * stat, byte charID, int time, int rings, int score)
{
#if RETRO_USE_PLUS
    stat->statID = 0;
    stat->name   = "ACT_CLEAR";
    memset(stat->data, 0, sizeof(stat->data));
    stat->data[0] = (void *)ZoneNames[zone];
    stat->data[1] = (void *)ActNames[act];
    stat->data[2] = (void *)PlayerNames[charID];
    stat->data[3] = 0;
    stat->data[4] = intToVoid(time);
    stat->data[5] = intToVoid(rings);
    stat->data[6] = intToVoid(score);
#else
    APICallback_TrackActClear(zoneID, act, charID, score, rings, time);
#endif
}
void TimeAttackData_TrackTAClear(byte actID, byte zone, StatInfo *stat, byte charID, int gameMode, int time)
{
#if RETRO_USE_PLUS
    stat->statID = 1;
    stat->name   = "TA_CLEAR";
    memset(stat->data, 0, sizeof(stat->data));
    stat->data[0] = (void *)ZoneNames[zone];
    stat->data[1] = (void *)ActNames[actID];
    stat->data[2] = (void *)PlayerNames[charID];
    stat->data[3] = (void *)ModeNames[gameMode];
    stat->data[4] = intToVoid(time);
#else
    APICallback_TrackTAClear(zoneID, act, charID, time);
#endif
}
void TimeAttackData_TrackEnemyDefeat(byte actID, byte zoneID, StatInfo *stat, byte charID, bool32 encore, int x, int y)
{
#if RETRO_USE_PLUS
    stat->statID   = 2;
    stat->name   = "ENEMY_DEFEAT";
    memset(stat->data, 0, sizeof(stat->data));
    stat->data[0]     = (void *)ZoneNames[zoneID];
    stat->data[1]     = (void *)ActNames[actID];
    stat->data[2]     = (void *)PlayerNames[charID];
    stat->data[3]     = intToVoid(encore);
    stat->data[4]     = intToVoid(x);
    stat->data[5]     = intToVoid(y);
#else
    APICallback_TrackEnemyDefeat(zoneID, act, charID, x, y);
#endif
}

void TimeAttackData_ClearOptions(void)
{
#if RETRO_USE_PLUS
    globals->menuParam[22] = 0;
    memset(&globals->menuParam[22] + 2, 0, 0x100);
    globals->menuParam[87]     = 0;
    globals->menuParam[88]     = 0;
    globals->menuParam[89]     = 0;
    globals->menuParam[92]     = 0;
    globals->menuParam[93]     = 0;
    globals->menuParam[94]     = 0;
    globals->gameMode          = MODE_MANIA;
    globals->suppressTitlecard = false;
    globals->suppressAutoMusic = false;
#else
    globals->menuParam[21] &= 0x0000FFFF;
    memset(&globals->menuParam[22], 0, 0x100);
    globals->menuParam[86]     = 0;
    globals->menuParam[87]     = 0;
    globals->menuParam[88]     = 0;
    globals->menuParam[90]     = 0;
    globals->menuParam[91]     = 0;
    globals->menuParam[92]     = 0;
    globals->gameMode          = MODE_NOSAVE;
    globals->suppressAutoMusic = false;
    globals->suppressTitlecard = false;
#endif
}
int TimeAttackData_GetManiaListPos(int zoneID, int playerID, int act)
{
    int listPos = 0;
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
                listPos = (playerID == 3) + 1 + 2 * zoneID;
            break;
        case 8:
        case 9: listPos = act + 2 * (zoneID + 1); break;
        case 10:
        case 11: listPos = act + (2 * zoneID + 3); break;
        default: break;
    }
    LogHelpers_Print("playerID = %d, zoneID = %d, act = %d", playerID, zoneID, act);
    LogHelpers_Print("listPos = %d", listPos);
    return listPos;
}
int TimeAttackData_GetEncoreListPos(int zoneID, int playerID, int act)
{
    int listPos = 0;
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
    LogHelpers_Print("playerID = %d, zoneID = %d, act = %d", playerID, zoneID, act);
    LogHelpers_Print("listPos = %d", listPos);
    return listPos;
}

void TimeAttackData_GetTimeFromValue(int time, int *minsPtr, int *secsPtr, int *millisecsPtr)
{
    int m;
    int s;
    int ms;
    m  = time / 6000;
    s  = time % 6000 / 100;
    ms = time % 100;
    if (minsPtr)
        *minsPtr = m;
    if (secsPtr)
        *secsPtr = s;
    if (millisecsPtr)
        *millisecsPtr = ms;
}
#if RETRO_USE_PLUS
int TimeAttackData_LoadCB(int statusCode)
{
    if (statusCode == STATUS_OK) {
        globals->taTableLoaded = STATUS_OK;
        API.Unknown31(globals->taTableID);
        LogHelpers_Print("Load Succeeded! Replay count: %d", API.GetUserDBUnknownCount(globals->taTableID));
    }
    else {
        LogHelpers_Print("Load Failed! Creating new Time Attack DB");
        TimeAttackData_ResetTimeAttackDB();
    }
    LogHelpers_Print("Replay DB Slot => %d, Load Status => %d", globals->taTableID, globals->taTableLoaded);

    if (TimeAttackData->loadCallback) {
        Entity *entStore = RSDK_sceneInfo->entity;
        if (TimeAttackData->loadEntityPtr)
            RSDK_sceneInfo->entity = TimeAttackData->loadEntityPtr;
        TimeAttackData->loadCallback(statusCode == STATUS_OK);
        RSDK_sceneInfo->entity        = entStore;
        TimeAttackData->loadCallback  = NULL;
        TimeAttackData->loadEntityPtr = NULL;
    }
    return 1;
}

void TimeAttackData_ResetTimeAttackDB(void)
{
    ushort id = API.InitUserDB("TimeAttackDB.bin", 2, "zoneID", 2, "act", 2, "characterID", 2, "encore", 4, "score", 4, "replayID", 0, NULL);
    globals->taTableID = id;
    if (id == 0xFFFF) {
        globals->taTableLoaded = STATUS_ERROR;
    }
    else {
        globals->taTableLoaded = STATUS_OK;
        if (!API.GetUserStorageNoSave()) {
            if (globals->saveLoaded == STATUS_OK)
                TimeAttackData_MigrateLegacyTADB();
        }
    }
}

void TimeAttackData_MigrateLegacyTADB(void)
{
    if (globals->saveLoaded == STATUS_OK) {
        // result = (GlobalVariables *)((char *)globals + 73892); //saveRAM[0x800]
        // if (globals != (GlobalVariables *)-73892) {
        TimeAttackData->dword1C = 1;
        LogHelpers_Print("===========================");
        LogHelpers_Print("Migrating Legacy TA Data...");
        LogHelpers_Print("===========================");

        for (int zone = 0; zone < 12; ++zone) {
            for (int act = 0; act < 2; ++act) {
                for (int charID = 0; charID < 3; ++charID) {
                    int off = 12;
                    for (int rank = 0; rank < 3; ++rank) {
                        ushort *saveRAM = NULL;
                        if (globals->saveLoaded == STATUS_OK)
                            saveRAM = (ushort *)&globals->saveRAM[0x800];
                        else
                            saveRAM = NULL;
                        int pos = act + 2 * (off + zone) - 10;
                        if (saveRAM[pos + rank + 2 * pos]) {
                            int time = saveRAM[pos + rank + 2 * pos];
                            LogHelpers_Print("Import: zone=%d act=%d charID=%d rank=%d -> %d", zone, act, charID + 1, rank, time);
                            TimeAttackData_AddTADBEntry(zone, charID + 1, act, MODE_MANIA, time, NULL);
                        }
                    }
                    off += 12;
                }
            }
            //}

            TimeAttackData->dword1C = 0;
        }
    }
}

int TimeAttackData_AddTimeAttackDBEntry(char zone, char charID, int act, char mode, int time)
{
    if (globals->taTableLoaded != STATUS_OK)
        return -1;

    ushort rowID = API.AddUserDBEntry(globals->taTableID);
    int encore   = mode & 1;
    API.SetUserDBValue(globals->taTableID, rowID, 2, "zoneID", &zone);
    API.SetUserDBValue(globals->taTableID, rowID, 2, "act", &act);
    API.SetUserDBValue(globals->taTableID, rowID, 2, "characterID", &charID);
    API.SetUserDBValue(globals->taTableID, rowID, 2, "encore", &encore);
    API.SetUserDBValue(globals->taTableID, rowID, 4, "score", &time);
    API.SetUserDBValue(globals->taTableID, rowID, 4, "replayID", NULL);
    uint uuid = API.GetUserDBEntryUUID(globals->taTableID, rowID);
    char buf[0x20];
    memset(buf, 0, 0x20 * sizeof(char));
    API.GetUserDBCreationTime(globals->taTableID, rowID, buf, 23, "%Y/%m/%d %H:%M:%S");
    LogHelpers_Print("Time Attack DB Added Entry");
    LogHelpers_Print("Created at %s", buf);
    LogHelpers_Print("Row ID: %d", rowID);
    LogHelpers_Print("UUID: %08X", uuid);
    return rowID;
}

int TimeAttackData_AddTADBEntry(char zone, char charID, int act, int mode, int time, void (*callback)(int))
{
    ushort row = TimeAttackData_AddTimeAttackDBEntry(zone, charID, act, mode, time);
    uint uuid  = API.GetUserDBEntryUUID(globals->taTableID, row);
    TimeAttackData_ConfigureTableView(zone, charID, act, mode);
    if (API.GetUserDBUnknownCount(globals->taTableID) > 3) {
        int unknown = API.GetUserDBUnknown(globals->taTableID, 3);
        API.RemoveDBEntry(globals->taTableID, unknown);
        TimeAttackData_ConfigureTableView(zone, charID, act, mode);
    }

    int c       = 0;
    int unknown = 0;
    for (c = 0; c < 3; ++c) {
        unknown = API.GetUserDBUnknown(globals->taTableID, c);
        if (API.GetUserDBEntryUUID(globals->taTableID, unknown) == uuid)
            break;
    }
    if (c == 3) {
        if (callback)
            callback(0);
        return 0;
    }

    TimeAttackData->uuid     = uuid;
    TimeAttackData->unknown  = unknown;
    TimeAttackData->field_14 = c + 1;
    if (TimeAttackData->dword1C == 1) {
        if (callback) {
            callback(1);
            return c + 1;
        }
    }
    else {
        TimeAttackData_SaveTimeAttackDB(callback);
    }
    return c + 1;
}

int TimeAttackData_SaveTimeAttackDB(void (*callback)(int))
{
    if (API.GetUserStorageNoSave() || globals->taTableID == 0xFFFF || globals->taTableLoaded != STATUS_OK) {
        if (callback)
            callback(0);
    }
    else {
        LogHelpers_Print("Saving Time Attack DB");
        TimeAttackData->saveEntityPtr = RSDK_sceneInfo->entity;
        TimeAttackData->saveCallback  = callback;
        API.SaveUserDB(globals->taTableID, TimeAttackData_SaveTimeAttackDB_CB);
    }
    return 1;
}

int TimeAttackData_SaveTimeAttackDB_CB(int statusCode)
{
    if (TimeAttackData->saveCallback) {
        Entity *entStore = RSDK_sceneInfo->entity;
        if (TimeAttackData->saveEntityPtr)
            RSDK_sceneInfo->entity = TimeAttackData->saveEntityPtr;
        TimeAttackData->saveCallback(statusCode == STATUS_OK);
        RSDK_sceneInfo->entity        = entStore;
        TimeAttackData->saveCallback  = NULL;
        TimeAttackData->saveEntityPtr = NULL;
    }
    return 1;
}

int TimeAttackData_SetScore(byte zone, byte charID, byte act, int encore, int dst)
{
    if ((byte)(dst - 1) > 2)
        return 0;

    if (!TimeAttackData->status || charID != TimeAttackData->characterID || zone != TimeAttackData->zoneID || act != TimeAttackData->act
        || encore != TimeAttackData->encore) {
        TimeAttackData_ConfigureTableView(zone, charID, act, encore);
    }

    int unknown = API.GetUserDBUnknown(globals->taTableID, dst - 1);
    if (unknown == -1)
        return 0;
    dst = 0;
    API.Unknown39(globals->taTableID, unknown, 4, "score", &dst);
    return dst;
}

void TimeAttackData_ConfigureTableView(byte zoneID, byte characterID, byte act, int encore)
{
    LogHelpers_Print("ConfigureTableView(%d, %d, %d, %d)", characterID, zoneID, act, encore);
    API.Unknown31(globals->taTableID);
    API.Unknown33(globals->taTableID, 2, "zoneID", &zoneID);
    API.Unknown33(globals->taTableID, 2, "act", &act);
    API.Unknown33(globals->taTableID, 2, "characterID", &characterID);
    API.Unknown33(globals->taTableID, 2, "encore", &encore);
    API.Unknown34(globals->taTableID, 4, "score", NULL);
    TimeAttackData->status      = 1;
    TimeAttackData->zoneID      = zoneID;
    TimeAttackData->act         = act;
    TimeAttackData->characterID = characterID;
    TimeAttackData->encore      = encore & 1;
}

void TimeAttackData_GetLeaderboardRank_CB(int status, int rank)
{
    if (status) {
        LogHelpers_Print("Got back leaderboard rank: %d. Not bad!", rank);
        TimeAttackData->rank = rank;
    }
}

void TimeAttackData_AddLeaderboardEntry(byte zone, char playerID, byte act, int mode, int time)
{
    StatInfo stat; 
    TimeAttackData_TrackTAClear(act, zone, &stat, playerID, mode, time);
    API.TryTrackStat(&stat);

    const char *leaderboardName = "";
    if (zone <= 11 && act <= 1 && playerID <= 5) {
        int id = act + 2 * zone - 1 + playerID + 4 * (act + 2 * zone) + 120;
        if (!mode)
            id = act + 2 * zone - 1 + playerID + 4 * (act + 2 * zone);
        leaderboardName = LeaderboardNames[id];
    }
    API.TrackScore(leaderboardName, time, TimeAttackData_GetLeaderboardRank_CB);
}

#else
void TimeAttackData_SaveTATime(byte zone, byte act, byte player, byte rank, ushort time)
{
    int *recordsRAM = NULL;
    if (globals->saveLoaded == STATUS_OK)
        recordsRAM = &globals->saveRAM[0x800];
    if (!recordsRAM)
        return;

    // playerID * (1 zones)
    // zone * (1 acts)
    // act * (3 ranks)
    ushort *times = (ushort *)&recordsRAM[(36 * player) - 15 + (3 * zone)] + 3 * act;
    for (int r = rank - 1; r < 2; ++r) {
        times[r] = times[r + 1];
    }

    times[rank - 1] = time;
}

#endif

void TimeAttackData_EditorDraw(void) {}
void TimeAttackData_EditorLoad(void) {}
void TimeAttackData_Serialize(void) {}
