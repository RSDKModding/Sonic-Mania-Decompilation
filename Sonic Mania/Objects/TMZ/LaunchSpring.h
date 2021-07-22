#ifndef OBJ_LAUNCHSPRING_H
#define OBJ_LAUNCHSPRING_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLaunchSpring;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLaunchSpring;

// Object Struct
extern ObjectLaunchSpring *LaunchSpring;

// Standard Entity Events
void LaunchSpring_Update(void);
void LaunchSpring_LateUpdate(void);
void LaunchSpring_StaticUpdate(void);
void LaunchSpring_Draw(void);
void LaunchSpring_Create(void* data);
void LaunchSpring_StageLoad(void);
void LaunchSpring_EditorDraw(void);
void LaunchSpring_EditorLoad(void);
void LaunchSpring_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LAUNCHSPRING_H
