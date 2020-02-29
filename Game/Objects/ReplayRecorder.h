#ifndef OBJ_REPLAYRECORDER_H
#define OBJ_REPLAYRECORDER_H

#include "../../SonicMania.h"

//Object Class
class ObjectReplayRecorder : public Object {
public:

};

//Entity Class
class EntityReplayRecorder : public Entity {
public:

};

//Object Entity
ObjectReplayRecorder ReplayRecorder;

//Entity Functions
void ReplayRecorder_Update();
void ReplayRecorder_EarlyUpdate();
void ReplayRecorder_LateUpdate();
void ReplayRecorder_Draw();
void ReplayRecorder_Setup(void* subtype);
void ReplayRecorder_StageLoad();
void ReplayRecorder_GetAttributes();

#endif //!OBJ_REPLAYRECORDER_H
