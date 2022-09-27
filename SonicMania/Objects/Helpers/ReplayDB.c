// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ReplayDB Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectReplayDB *ReplayDB;

void ReplayDB_Update(void) {}

void ReplayDB_LateUpdate(void) {}

void ReplayDB_StaticUpdate(void) {}

void ReplayDB_Draw(void) {}

void ReplayDB_Create(void *data) {}

void ReplayDB_StageLoad(void) {}

void ReplayDB_CreateDB(void)
{
    globals->replayTableID = API.InitUserDB("ReplayDB.bin", DBVAR_UINT32, "score", DBVAR_UINT8, "zoneID", DBVAR_UINT8, "act", DBVAR_UINT8,
                                            "characterID", DBVAR_UINT8, "encore", DBVAR_UINT32, "zoneSortVal", NULL);

    if (globals->replayTableID == -1)
        globals->replayTableLoaded = STATUS_ERROR;
    else
        globals->replayTableLoaded = STATUS_OK;
}

void ReplayDB_LoadDB(void (*callback)(bool32 success))
{
    if ((globals->replayTableID != -1 && globals->replayTableLoaded == STATUS_OK) || globals->replayTableLoaded == STATUS_CONTINUE) {
        if (callback)
            callback(false);
    }
    else {
        LogHelpers_Print("Loading Replay DB");
        globals->replayTableLoaded = STATUS_CONTINUE;

        ReplayDB->loadEntity   = SceneInfo->entity;
        ReplayDB->loadCallback = callback;
        globals->replayTableID = API.LoadUserDB("ReplayDB.bin", ReplayDB_LoadDBCallback);

        if (globals->replayTableID == -1) {
            LogHelpers_Print("Couldn't claim a slot for loading %s", "ReplayDB.bin");
            globals->replayTableLoaded = STATUS_ERROR;
        }
    }
}

void ReplayDB_SaveDB(void (*callback)(bool32 success))
{
    if (API_GetNoSave() || globals->replayTableID == (uint16)-1 || globals->replayTableLoaded != STATUS_OK) {
        if (callback)
            callback(false);
    }
    else {
        LogHelpers_Print("Saving Replay DB");
        ReplayDB->saveEntity   = SceneInfo->entity;
        ReplayDB->saveCallback = callback;
        API.SaveUserDB(globals->replayTableID, ReplayDB_SaveDBCallback);
    }
}

uint32 ReplayDB_AddReplay(uint8 zoneID, uint8 act, uint8 characterID, int32 score, uint8 encore)
{
    if (globals->replayTableLoaded == STATUS_OK) {
        uint32 rowID       = API.AddUserDBRow(globals->replayTableID);
        int32 zoneStortVal = (score & 0x3FFFFFF) | (((zoneID << 2) | (act & 1) | ((encore & 1) << 1)) << 26);

        API.SetUserDBValue(globals->replayTableID, rowID, DBVAR_UINT32, "score", &score);
        API.SetUserDBValue(globals->replayTableID, rowID, DBVAR_UINT8, "zoneID", &zoneID);
        API.SetUserDBValue(globals->replayTableID, rowID, DBVAR_UINT8, "act", &act);
        API.SetUserDBValue(globals->replayTableID, rowID, DBVAR_UINT8, "characterID", &characterID);
        API.SetUserDBValue(globals->replayTableID, rowID, DBVAR_UINT8, "encore", &encore);
        API.SetUserDBValue(globals->replayTableID, rowID, DBVAR_UINT32, "zoneSortVal", &zoneStortVal);

        uint32 UUID = API.GetUserDBRowUUID(globals->replayTableID, rowID);
        char createTime[24];
        sprintf_s(createTime, (int32)sizeof(createTime), "");
        API.GetUserDBRowCreationTime(globals->replayTableID, rowID, createTime, sizeof(createTime) - 1, "%Y/%m/%d %H:%M:%S");

        LogHelpers_Print("Replay DB Added Entry");
        LogHelpers_Print("Created at %s", createTime);
        LogHelpers_Print("Row ID: %d", rowID);
        LogHelpers_Print("UUID: %08X", UUID);

        return rowID;
    }

    return -1;
}

void ReplayDB_DeleteReplay(int32 row, void (*callback)(bool32 success), bool32 useAltCB)
{
    int32 id       = API.GetUserDBRowUUID(globals->replayTableID, row);
    int32 replayID = 0;

    ReplayDB->deleteEntity   = SceneInfo->entity;
    ReplayDB->deleteCallback = callback;
    API.RemoveDBRow(globals->replayTableID, row);
    TimeAttackData->loaded = false;

    API.SetupUserDBRowSorting(globals->taTableID);
    API.AddRowSortFilter(globals->taTableID, DBVAR_UINT32, "replayID", &id);

    int32 count = API.GetSortedUserDBRowCount(globals->taTableID);
    for (int32 i = 0; i < count; ++i) {
        uint32 uuid = API.GetSortedUserDBRowID(globals->taTableID, i);
        LogHelpers_Print("Deleting Time Attack replay from row #%d", uuid);
        API.SetUserDBValue(globals->taTableID, uuid, DBVAR_UINT32, "replayID", &replayID);
    }

    char filename[0x20];
    sprintf_s(filename, (int32)sizeof(filename), "Replay_%08X.bin", id);
    if (!useAltCB)
        API.DeleteUserFile(filename, ReplayDB_DeleteReplay_CB);
    else
        API.DeleteUserFile(filename, ReplayDB_DeleteReplaySave2_CB);
}

void ReplayDB_DeleteReplay_CB(int32 status)
{
    LogHelpers_Print("DeleteReplay_CB(%d)", status);

    API.SaveUserDB(globals->replayTableID, ReplayDB_DeleteReplaySave_CB);
}

void ReplayDB_DeleteReplaySave_CB(int32 status)
{
    LogHelpers_Print("DeleteReplaySave_CB(%d)", status);

    API.SaveUserDB(globals->taTableID, ReplayDB_DeleteReplaySave2_CB);
}

void ReplayDB_DeleteReplaySave2_CB(int32 status)
{
    LogHelpers_Print("DeleteReplaySave2_CB(%d)", status);

    if (ReplayDB->deleteCallback) {
        Entity *store = SceneInfo->entity;
        if (ReplayDB->deleteEntity)
            SceneInfo->entity = ReplayDB->deleteEntity;
        ReplayDB->deleteCallback(status == STATUS_OK);
        SceneInfo->entity = store;

        ReplayDB->deleteCallback = NULL;
        ReplayDB->deleteEntity   = NULL;
    }
}

void ReplayDB_LoadDBCallback(int32 status)
{
    if (status == STATUS_OK) {
        globals->replayTableLoaded = STATUS_OK;
        API.SetupUserDBRowSorting(globals->replayTableID);
        LogHelpers_Print("Load Succeeded! Replay count: %d", API.GetSortedUserDBRowCount(globals->replayTableID));
    }
    else {
        LogHelpers_Print("Load Failed! Creating new Replay DB");
        ReplayDB_CreateDB();
    }

    LogHelpers_Print("Replay DB Slot => %d, Load Status => %d", globals->replayTableID, globals->replayTableLoaded);

    if (ReplayDB->loadCallback) {
        Entity *store = SceneInfo->entity;
        if (ReplayDB->loadEntity)
            SceneInfo->entity = ReplayDB->loadEntity;
        ReplayDB->loadCallback(status == STATUS_OK);
        SceneInfo->entity = store;

        ReplayDB->loadCallback = NULL;
        ReplayDB->loadEntity   = NULL;
    }
}

void ReplayDB_SaveDBCallback(int32 status)
{
    if (ReplayDB->saveCallback) {
        Entity *store = SceneInfo->entity;
        if (ReplayDB->saveEntity)
            SceneInfo->entity = ReplayDB->saveEntity;
        ReplayDB->saveCallback(status == STATUS_OK);
        SceneInfo->entity = store;

        ReplayDB->saveCallback = NULL;
        ReplayDB->saveEntity   = NULL;
    }
}

void ReplayDB_LoadCallback(bool32 success) {}

int32 ReplayDB_Buffer_PackEntry(uint8 *compressed, void *uncompressed)
{
    ReplayFrame *framePtr = (ReplayFrame *)uncompressed;

    compressed[0]    = framePtr->info;
    compressed[1]    = framePtr->changedValues;
    bool32 forcePack = framePtr->info == REPLAY_INFO_STATECHANGE || framePtr->info == REPLAY_INFO_PASSEDGATE;
    uint8 changes    = framePtr->changedValues;

    uint8 *compressedBuffer = &compressed[2];

    // input
    if (forcePack || (changes & REPLAY_CHANGED_INPUT)) {
        *compressedBuffer = framePtr->inputs;
        ++compressedBuffer;
    }

    // position
    if (forcePack || (changes & REPLAY_CHANGED_POS)) {
        *((int32 *)compressedBuffer) = framePtr->position.x;
        compressedBuffer += sizeof(int32);

        *((int32 *)compressedBuffer) = framePtr->position.y;
        compressedBuffer += sizeof(int32);
    }

    // velocity
    if (forcePack || (changes & REPLAY_CHANGED_VEL)) {
        *((int32 *)compressedBuffer) = framePtr->velocity.x;
        compressedBuffer += sizeof(int32);

        *((int32 *)compressedBuffer) = framePtr->velocity.y;
        compressedBuffer += sizeof(int32);
    }

    // rotation
    if (forcePack || (changes & REPLAY_CHANGED_ROT)) {
        *compressedBuffer = framePtr->rotation >> 1;
        compressedBuffer += sizeof(uint8);
    }

    // direction
    if (forcePack || (changes & REPLAY_CHANGED_DIR)) {
        *compressedBuffer = framePtr->direction;
        ++compressedBuffer;
    }

    // anim
    if (forcePack || (changes & REPLAY_CHANGED_ANIM)) {
        *compressedBuffer = framePtr->anim;
        ++compressedBuffer;
    }

    // frame
    if (forcePack || (changes & REPLAY_CHANGED_FRAME)) {
        *compressedBuffer = framePtr->frame;
        ++compressedBuffer;
    }

    return (int32)(compressedBuffer - compressed);
}

int32 ReplayDB_Buffer_UnpackEntry(void *uncompressed, uint8 *compressed)
{
    ReplayFrame *framePtr = (ReplayFrame *)uncompressed;

    // compress state
    framePtr->info = compressed[0];

    bool32 forceUnpack      = *compressed == REPLAY_INFO_STATECHANGE || *compressed == REPLAY_INFO_PASSEDGATE;
    uint8 changes           = compressed[1];
    framePtr->changedValues = changes;

    uint8 *compressedBuffer = &compressed[2];

    // input
    if (forceUnpack || (changes & REPLAY_CHANGED_INPUT)) {
        framePtr->inputs = *compressedBuffer++;
    }

    // position
    if (forceUnpack || (changes & REPLAY_CHANGED_POS)) {
        int32 x = *(int32 *)compressedBuffer;
        compressedBuffer += sizeof(int32);

        int32 y = *(int32 *)compressedBuffer;
        compressedBuffer += sizeof(int32);

        framePtr->position.x = x;
        framePtr->position.y = y;
    }

    // velocity
    if (forceUnpack || (changes & REPLAY_CHANGED_VEL)) {
        int32 x = *(int32 *)compressedBuffer;
        compressedBuffer += sizeof(int32);

        int32 y = *(int32 *)compressedBuffer;
        compressedBuffer += sizeof(int32);

        framePtr->velocity.x = x;
        framePtr->velocity.y = y;
    }

    // rotation
    if (forceUnpack || (changes & REPLAY_CHANGED_ROT)) {
        int32 rotation     = *compressedBuffer++;
        framePtr->rotation = rotation << 1;
    }

    // direction
    if (forceUnpack || (changes & REPLAY_CHANGED_DIR)) {
        framePtr->direction = *compressedBuffer++;
    }

    // anim
    if (forceUnpack || (changes & REPLAY_CHANGED_ANIM)) {
        framePtr->anim = *compressedBuffer++;
    }

    // frame
    if (forceUnpack || (changes & REPLAY_CHANGED_FRAME)) {
        framePtr->frame = *compressedBuffer++;
    }

    return (int32)(compressedBuffer - compressed);
}

#if RETRO_INCLUDE_EDITOR
void ReplayDB_EditorDraw(void) {}

void ReplayDB_EditorLoad(void) {}
#endif

void ReplayDB_Serialize(void) {}
#endif
