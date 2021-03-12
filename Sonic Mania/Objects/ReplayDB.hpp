#ifndef OBJ_REPLAYDB_H
#define OBJ_REPLAYDB_H

#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
// Object Class
struct ObjectReplayDB : Object {
    Entity* loadEntity;
    void(*loadCallback)(bool32);
    Entity *saveEntity;
    void (*saveCallback)(bool32);
    Entity *deleteEntity;
    void (*deleteCallback)(bool32);
};

// Entity Class
struct EntityReplayDB : Entity {

};

// Object Struct
extern ObjectReplayDB *ReplayDB;

// Standard Entity Events
void ReplayDB_Update();
void ReplayDB_LateUpdate();
void ReplayDB_StaticUpdate();
void ReplayDB_Draw();
void ReplayDB_Create(void* data);
void ReplayDB_StageLoad();
void ReplayDB_EditorDraw();
void ReplayDB_EditorLoad();
void ReplayDB_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_REPLAYDB_H
