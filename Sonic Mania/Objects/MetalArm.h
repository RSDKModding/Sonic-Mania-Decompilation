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
void MetalArm_Update(void);
void MetalArm_LateUpdate(void);
void MetalArm_StaticUpdate(void);
void MetalArm_Draw(void);
void MetalArm_Create(void* data);
void MetalArm_StageLoad(void);
void MetalArm_EditorDraw(void);
void MetalArm_EditorLoad(void);
void MetalArm_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_METALARM_H
