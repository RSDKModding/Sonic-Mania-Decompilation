#ifndef OBJ_REPLAYDB_H
#define OBJ_REPLAYDB_H

//Object Class
class ReplayDB : Object {

};

//Entity Class
class EntityReplayDB : Entity {

};

//Entity Functions
void ReplayDB_Update();
void ReplayDB_EarlyUpdate();
void ReplayDB_LateUpdate();
void ReplayDB_Draw();
void ReplayDB_Setup(void* subtype);
void ReplayDB_StageLoad();
void ReplayDB_GetAttributes();

#endif //!OBJ_REPLAYDB_H
