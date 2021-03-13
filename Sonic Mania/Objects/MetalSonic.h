#ifndef OBJ_METALSONIC_H
#define OBJ_METALSONIC_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMetalSonic;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMetalSonic;

// Object Struct
extern ObjectMetalSonic *MetalSonic;

// Standard Entity Events
void MetalSonic_Update();
void MetalSonic_LateUpdate();
void MetalSonic_StaticUpdate();
void MetalSonic_Draw();
void MetalSonic_Create(void* data);
void MetalSonic_StageLoad();
void MetalSonic_EditorDraw();
void MetalSonic_EditorLoad();
void MetalSonic_Serialize();

// Extra Entity Functions


#endif //!OBJ_METALSONIC_H
