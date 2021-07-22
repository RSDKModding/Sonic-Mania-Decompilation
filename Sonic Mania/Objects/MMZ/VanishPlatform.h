#ifndef OBJ_VANISHPLATFORM_H
#define OBJ_VANISHPLATFORM_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectVanishPlatform;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityVanishPlatform;

// Object Struct
extern ObjectVanishPlatform *VanishPlatform;

// Standard Entity Events
void VanishPlatform_Update(void);
void VanishPlatform_LateUpdate(void);
void VanishPlatform_StaticUpdate(void);
void VanishPlatform_Draw(void);
void VanishPlatform_Create(void* data);
void VanishPlatform_StageLoad(void);
void VanishPlatform_EditorDraw(void);
void VanishPlatform_EditorLoad(void);
void VanishPlatform_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_VANISHPLATFORM_H
