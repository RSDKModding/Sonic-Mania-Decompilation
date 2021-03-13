#ifndef OBJ_LIGHTBARRIER_H
#define OBJ_LIGHTBARRIER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLightBarrier;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLightBarrier;

// Object Struct
extern ObjectLightBarrier *LightBarrier;

// Standard Entity Events
void LightBarrier_Update();
void LightBarrier_LateUpdate();
void LightBarrier_StaticUpdate();
void LightBarrier_Draw();
void LightBarrier_Create(void* data);
void LightBarrier_StageLoad();
void LightBarrier_EditorDraw();
void LightBarrier_EditorLoad();
void LightBarrier_Serialize();

// Extra Entity Functions


#endif //!OBJ_LIGHTBARRIER_H
