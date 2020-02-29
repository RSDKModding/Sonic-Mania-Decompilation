#ifndef OBJ_REPLAYDB_H
#define OBJ_REPLAYDB_H

#include "../../SonicMania.h"

//Object Class
class ObjectReplayDB : public Object {
public:

};

//Entity Class
class EntityReplayDB : public Entity {
public:

};

//Object Entity
ObjectReplayDB ReplayDB;

//Entity Functions
void ReplayDB_Update();
void ReplayDB_EarlyUpdate();
void ReplayDB_LateUpdate();
void ReplayDB_Draw();
void ReplayDB_Setup(void* subtype);
void ReplayDB_StageLoad();
void ReplayDB_GetAttributes();

#endif //!OBJ_REPLAYDB_H
