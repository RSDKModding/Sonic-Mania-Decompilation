#ifndef OBJ_REPLAYRECORDER_H
#define OBJ_REPLAYRECORDER_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectReplayRecorder;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityReplayRecorder;

// Object Struct
extern ObjectReplayRecorder *ReplayRecorder;

// Standard Entity Events
void ReplayRecorder_Update(void);
void ReplayRecorder_LateUpdate(void);
void ReplayRecorder_StaticUpdate(void);
void ReplayRecorder_Draw(void);
void ReplayRecorder_Create(void* data);
void ReplayRecorder_StageLoad(void);
void ReplayRecorder_EditorDraw(void);
void ReplayRecorder_EditorLoad(void);
void ReplayRecorder_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_REPLAYRECORDER_H
