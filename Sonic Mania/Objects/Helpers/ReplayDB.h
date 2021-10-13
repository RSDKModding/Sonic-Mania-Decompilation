#ifndef OBJ_REPLAYDB_H
#define OBJ_REPLAYDB_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    Entity *loadEntity;
    void (*loadCallback)(bool32);
    Entity *saveEntity;
    void (*saveCallback)(bool32);
    Entity *deleteEntity;
    void (*deleteCallback)(bool32);
} ObjectReplayDB;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityReplayDB;

// Object Struct
extern ObjectReplayDB *ReplayDB;

// Standard Entity Events
void ReplayDB_Update(void);
void ReplayDB_LateUpdate(void);
void ReplayDB_StaticUpdate(void);
void ReplayDB_Draw(void);
void ReplayDB_Create(void* data);
void ReplayDB_StageLoad(void);
void ReplayDB_EditorDraw(void);
void ReplayDB_EditorLoad(void);
void ReplayDB_Serialize(void);

// Extra Entity Functions
int ReplayDB_Buffer_PackEntry(byte *compressed, byte *uncompressed);
int ReplayDB_Buffer_UnpackEntry(byte *uncompressed, byte *compressed);
#endif

#endif //!OBJ_REPLAYDB_H
