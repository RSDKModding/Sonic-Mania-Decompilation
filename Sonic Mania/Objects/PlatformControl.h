#ifndef OBJ_PLATFORMCONTROL_H
#define OBJ_PLATFORMCONTROL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPlatformControl;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPlatformControl;

// Object Struct
extern ObjectPlatformControl *PlatformControl;

// Standard Entity Events
void PlatformControl_Update();
void PlatformControl_LateUpdate();
void PlatformControl_StaticUpdate();
void PlatformControl_Draw();
void PlatformControl_Create(void* data);
void PlatformControl_StageLoad();
void PlatformControl_EditorDraw();
void PlatformControl_EditorLoad();
void PlatformControl_Serialize();

// Extra Entity Functions


#endif //!OBJ_PLATFORMCONTROL_H
