#ifndef OBJ_REPLAYDB_H
#define OBJ_REPLAYDB_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectReplayDB : Object {

};

// Entity Class
struct EntityReplayDB : Entity {

};

// Object Entity
extern ObjectReplayDB ReplayDB;

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


#endif //!OBJ_REPLAYDB_H
