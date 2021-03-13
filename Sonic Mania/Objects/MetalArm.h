#ifndef OBJ_METALARM_H
#define OBJ_METALARM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMetalArm;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMetalArm;

// Object Struct
extern ObjectMetalArm *MetalArm;

// Standard Entity Events
void MetalArm_Update();
void MetalArm_LateUpdate();
void MetalArm_StaticUpdate();
void MetalArm_Draw();
void MetalArm_Create(void* data);
void MetalArm_StageLoad();
void MetalArm_EditorDraw();
void MetalArm_EditorLoad();
void MetalArm_Serialize();

// Extra Entity Functions


#endif //!OBJ_METALARM_H
