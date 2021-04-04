#ifndef OBJ_PULLSWITCH_H
#define OBJ_PULLSWITCH_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPullSwitch;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPullSwitch;

// Object Struct
extern ObjectPullSwitch *PullSwitch;

// Standard Entity Events
void PullSwitch_Update(void);
void PullSwitch_LateUpdate(void);
void PullSwitch_StaticUpdate(void);
void PullSwitch_Draw(void);
void PullSwitch_Create(void* data);
void PullSwitch_StageLoad(void);
void PullSwitch_EditorDraw(void);
void PullSwitch_EditorLoad(void);
void PullSwitch_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PULLSWITCH_H
