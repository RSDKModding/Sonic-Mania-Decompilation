#ifndef OBJ_REPLAYRECORDER_H
#define OBJ_REPLAYRECORDER_H

#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
// Object Class
struct ObjectReplayRecorder : Object {

};

// Entity Class
struct EntityReplayRecorder : Entity {

};

// Object Struct
extern ObjectReplayRecorder *ReplayRecorder;

// Standard Entity Events
void ReplayRecorder_Update();
void ReplayRecorder_LateUpdate();
void ReplayRecorder_StaticUpdate();
void ReplayRecorder_Draw();
void ReplayRecorder_Create(void* data);
void ReplayRecorder_StageLoad();
void ReplayRecorder_EditorDraw();
void ReplayRecorder_EditorLoad();
void ReplayRecorder_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_REPLAYRECORDER_H
