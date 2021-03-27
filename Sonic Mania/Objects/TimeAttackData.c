#include "../SonicMania.h"

ObjectTimeAttackData *TimeAttackData;

void TimeAttackData_Update() {}
void TimeAttackData_LateUpdate() {}
void TimeAttackData_StaticUpdate() {}
void TimeAttackData_Draw() {}
void TimeAttackData_Create(void *data) {}
void TimeAttackData_StageLoad() {}

#if RETRO_USE_PLUS
int TimeAttackData_LoadCB(int statusCode)
{
    if (statusCode == 200) {
        globals->taTableLoaded = 200;
        User.Unknown31(globals->taTableID);
        Game_Print("Load Succeeded! Replay count: %d", User.GetUserDBUnknownCount(globals->taTableID));
    }
    else {
        Game_Print("Load Failed! Creating new Time Attack DB");
        TimeAttackData_ResetTimeAttackDB();
    }
    Game_Print("Replay DB Slot => %d, Load Status => %d", globals->taTableID, globals->taTableLoaded);

    if (TimeAttackData->loadCallback) {
        Entity *entStore = RSDK_sceneInfo->entity;
        if (TimeAttackData->loadEntityPtr)
            RSDK_sceneInfo->entity = TimeAttackData->loadEntityPtr;
        TimeAttackData->loadCallback(statusCode == 200);
        RSDK_sceneInfo->entity        = entStore;
        TimeAttackData->loadCallback  = NULL;
        TimeAttackData->loadEntityPtr = NULL;
    }
    return 1;
}

void TimeAttackData_ResetTimeAttackDB()
{
    ushort id = User.InitUserDB("TimeAttackDB.bin", 2, "zoneID", 2, "act", 2, "characterID", 2, "encore", 4, "score", 4, "replayID", 0, NULL);
    globals->taTableID = id;
    if (id == -1) {
        globals->taTableLoaded = 500;
    }
    else {
        globals->taTableLoaded = 200;
        if (!User.GetUserStorageNoSave()) {
            if (globals->saveLoaded == 200)
                TimeAttackData_MigrateLegacyTADB();
        }
    }
}

void TimeAttackData_MigrateLegacyTADB()
{
    if (globals->saveLoaded == 200) {
        // result = (GlobalVariables *)((char *)globals + 73892); //saveRAM[0x800]
        // if (globals != (GlobalVariables *)-73892) {
        TimeAttackData->dword1C = 1;
        Game_Print("===========================");
        Game_Print("Migrating Legacy TA Data...");
        Game_Print("===========================");

        for (int zone = 0; zone < 12; ++zone) {
            for (int act = 0; act < 2; ++act) {
                for (int charID = 0; charID < 3; ++charID) {
                    int off = 12;
                    for (int rank = 0; rank < 3; ++rank) {
                        ushort *saveRAM = NULL;
                        if (globals->saveLoaded == 200)
                            saveRAM = (ushort *)&globals->saveRAM[0x800];
                        else
                            saveRAM = NULL;
                        int pos = act + 2 * (off + zone) - 10;
                        if (saveRAM[pos + rank + 2 * pos]) {
                            int time = saveRAM[pos + rank + 2 * pos];
                            Game_Print("Import: zone=%d act=%d charID=%d rank=%d -> %d", zone, act, charID + 1, rank, time);
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
    if (globals->taTableLoaded != 200)
        return -1;

    ushort rowID = User.AddUserDBEntry(globals->taTableID);
    int encore   = mode & 1;
    User.SetUserDBValue(globals->taTableID, rowID, 2, "zoneID", &zone);
    User.SetUserDBValue(globals->taTableID, rowID, 2, "act", &act);
    User.SetUserDBValue(globals->taTableID, rowID, 2, "characterID", &charID);
    User.SetUserDBValue(globals->taTableID, rowID, 2, "encore", &encore);
    User.SetUserDBValue(globals->taTableID, rowID, 4, "score", &time);
    User.SetUserDBValue(globals->taTableID, rowID, 4, "replayID", NULL);
    uint uuid = User.GetUserDBEntryUUID(globals->taTableID, rowID);
    char buf[0x20];
    memset(buf, 0, 0x20 * sizeof(char));
    User.GetUserDBCreationTime(globals->taTableID, rowID, buf, 23, "%Y/%m/%d %H:%M:%S");
    Game_Print("Time Attack DB Added Entry");
    Game_Print("Created at %s", buf);
    Game_Print("Row ID: %d", rowID);
    Game_Print("UUID: %08X", uuid);
    return rowID;
}

int TimeAttackData_AddTADBEntry(char zone, char charID, int act, int mode, int time, void (*callback)(int))
{
    ushort row = TimeAttackData_AddTimeAttackDBEntry(zone, charID, act, mode, time);
    uint uuid  = User.GetUserDBEntryUUID(globals->taTableID, row);
    TimeAttackData_ConfigureTableView(zone, charID, act, mode);
    if (User.GetUserDBUnknownCount(globals->taTableID) > 3) {
        int unknown = User.GetUserDBUnknown(globals->taTableID, 3);
        User.RemoveDBEntry(globals->taTableID, unknown);
        TimeAttackData_ConfigureTableView(zone, charID, act, mode);
    }

    int c       = 0;
    int unknown = 0;
    for (c = 0; c < 3; ++c) {
        unknown = User.GetUserDBUnknown(globals->taTableID, c);
        if (User.GetUserDBEntryUUID(globals->taTableID, unknown) == uuid)
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
    if (User.GetUserStorageNoSave() || globals->taTableID == 0xFFFF || globals->taTableLoaded != 200) {
        if (callback)
            callback(0);
    }
    else {
        Game_Print("Saving Time Attack DB");
        TimeAttackData->saveEntityPtr = RSDK_sceneInfo->entity;
        TimeAttackData->saveCallback  = callback;
        User.SaveUserDB(globals->taTableID, TimeAttackData_SaveTimeAttackDB_CB);
    }
    return 1;
}

int TimeAttackData_SaveTimeAttackDB_CB(int statusCode)
{
    if (TimeAttackData->saveCallback) {
        Entity *entStore = RSDK_sceneInfo->entity;
        if (TimeAttackData->saveEntityPtr)
            RSDK_sceneInfo->entity = TimeAttackData->saveEntityPtr;
        TimeAttackData->saveCallback(statusCode == 200);
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

    int unknown = User.GetUserDBUnknown(globals->taTableID, dst - 1);
    if (unknown == -1)
        return 0;
    dst = 0;
    User.Unknown39(globals->taTableID, unknown, 4, "score", &dst);
    return dst;
}

void TimeAttackData_ConfigureTableView(byte zoneID, byte characterID, byte act, int encore)
{
    Game_Print("ConfigureTableView(%d, %d, %d, %d)", characterID, zoneID, act, encore);
    User.Unknown31(globals->taTableID);
    User.Unknown33(globals->taTableID, 2, "zoneID", &zoneID);
    User.Unknown33(globals->taTableID, 2, "act", &act);
    User.Unknown33(globals->taTableID, 2, "characterID", &characterID);
    User.Unknown33(globals->taTableID, 2, "encore", &encore);
    User.Unknown34(globals->taTableID, 4, "score", NULL);
    TimeAttackData->status      = 1;
    TimeAttackData->zoneID      = zoneID;
    TimeAttackData->act         = act;
    TimeAttackData->characterID = characterID;
    TimeAttackData->encore      = encore & 1;
}

#endif

void TimeAttackData_EditorDraw() {}
void TimeAttackData_EditorLoad() {}
void TimeAttackData_Serialize() {}