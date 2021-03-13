#ifndef OBJ_TIMEPOST_H
#define OBJ_TIMEPOST_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTimePost;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTimePost;

// Object Struct
extern ObjectTimePost *TimePost;

// Standard Entity Events
void TimePost_Update();
void TimePost_LateUpdate();
void TimePost_StaticUpdate();
void TimePost_Draw();
void TimePost_Create(void* data);
void TimePost_StageLoad();
void TimePost_EditorDraw();
void TimePost_EditorLoad();
void TimePost_Serialize();

// Extra Entity Functions


#endif //!OBJ_TIMEPOST_H
