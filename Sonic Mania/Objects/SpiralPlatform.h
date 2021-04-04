#ifndef OBJ_SPIRALPLATFORM_H
#define OBJ_SPIRALPLATFORM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpiralPlatform;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpiralPlatform;

// Object Struct
extern ObjectSpiralPlatform *SpiralPlatform;

// Standard Entity Events
void SpiralPlatform_Update(void);
void SpiralPlatform_LateUpdate(void);
void SpiralPlatform_StaticUpdate(void);
void SpiralPlatform_Draw(void);
void SpiralPlatform_Create(void* data);
void SpiralPlatform_StageLoad(void);
void SpiralPlatform_EditorDraw(void);
void SpiralPlatform_EditorLoad(void);
void SpiralPlatform_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SPIRALPLATFORM_H
