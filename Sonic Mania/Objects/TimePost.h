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
void TimePost_Update(void);
void TimePost_LateUpdate(void);
void TimePost_StaticUpdate(void);
void TimePost_Draw(void);
void TimePost_Create(void* data);
void TimePost_StageLoad(void);
void TimePost_EditorDraw(void);
void TimePost_EditorLoad(void);
void TimePost_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TIMEPOST_H
