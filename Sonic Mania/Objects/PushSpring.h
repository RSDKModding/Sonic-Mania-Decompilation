#ifndef OBJ_PUSHSPRING_H
#define OBJ_PUSHSPRING_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPushSpring;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPushSpring;

// Object Struct
extern ObjectPushSpring *PushSpring;

// Standard Entity Events
void PushSpring_Update(void);
void PushSpring_LateUpdate(void);
void PushSpring_StaticUpdate(void);
void PushSpring_Draw(void);
void PushSpring_Create(void* data);
void PushSpring_StageLoad(void);
void PushSpring_EditorDraw(void);
void PushSpring_EditorLoad(void);
void PushSpring_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PUSHSPRING_H
