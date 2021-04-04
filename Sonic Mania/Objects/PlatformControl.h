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
void PlatformControl_Update(void);
void PlatformControl_LateUpdate(void);
void PlatformControl_StaticUpdate(void);
void PlatformControl_Draw(void);
void PlatformControl_Create(void* data);
void PlatformControl_StageLoad(void);
void PlatformControl_EditorDraw(void);
void PlatformControl_EditorLoad(void);
void PlatformControl_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PLATFORMCONTROL_H
