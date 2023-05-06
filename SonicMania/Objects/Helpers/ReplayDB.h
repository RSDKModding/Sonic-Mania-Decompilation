#ifndef OBJ_REPLAYDB_H
#define OBJ_REPLAYDB_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectReplayDB {
    RSDK_OBJECT
    Entity *loadEntity;
    void (*loadCallback)(bool32 success);
    Entity *saveEntity;
    void (*saveCallback)(bool32 success);
    Entity *deleteEntity;
    void (*deleteCallback)(bool32 success);
};

// Entity Class
struct EntityReplayDB {
    RSDK_ENTITY
};

// Object Struct
extern ObjectReplayDB *ReplayDB;

// Standard Entity Events
void ReplayDB_Update(void);
void ReplayDB_LateUpdate(void);
void ReplayDB_StaticUpdate(void);
void ReplayDB_Draw(void);
void ReplayDB_Create(void *data);
void ReplayDB_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ReplayDB_EditorDraw(void);
void ReplayDB_EditorLoad(void);
#endif
void ReplayDB_Serialize(void);

// Extra Entity Functions
void ReplayDB_CreateDB(void);
void ReplayDB_LoadDB(void (*callback)(bool32 success));
void ReplayDB_SaveDB(void (*callback)(bool32 success));
uint32 ReplayDB_AddReplay(uint8 zoneID, uint8 act, uint8 characterID, int32 score, uint8 encore);
void ReplayDB_DeleteReplay(int32 row, void (*callback)(bool32 success), bool32 useAltCB);
void ReplayDB_DeleteReplay_CB(int32 status);
void ReplayDB_DeleteReplaySave_CB(int32 status);
void ReplayDB_DeleteReplaySave2_CB(int32 status);
void ReplayDB_LoadDBCallback(int32 status);
void ReplayDB_SaveDBCallback(int32 status);
void ReplayDB_LoadCallback(bool32 success);

int32 ReplayDB_Buffer_PackEntry(uint8 *compressed, void *uncompressed);
int32 ReplayDB_Buffer_UnpackEntry(void *uncompressed, uint8 *compressed);
#endif

#endif //! OBJ_REPLAYDB_H
