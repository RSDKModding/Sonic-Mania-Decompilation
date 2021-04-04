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
void MetalSonic_Update(void);
void MetalSonic_LateUpdate(void);
void MetalSonic_StaticUpdate(void);
void MetalSonic_Draw(void);
void MetalSonic_Create(void* data);
void MetalSonic_StageLoad(void);
void MetalSonic_EditorDraw(void);
void MetalSonic_EditorLoad(void);
void MetalSonic_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_METALSONIC_H
