#ifndef OBJ_REPLAYRECORDER_H
#define OBJ_REPLAYRECORDER_H

//Object Class
class ReplayRecorder : Object {

};

//Entity Class
class EntityReplayRecorder : Entity {

};

//Entity Functions
void ReplayRecorder_Update();
void ReplayRecorder_EarlyUpdate();
void ReplayRecorder_LateUpdate();
void ReplayRecorder_Draw();
void ReplayRecorder_Setup(void* subtype);
void ReplayRecorder_StageLoad();
void ReplayRecorder_GetAttributes();

#endif //!OBJ_REPLAYRECORDER_H
