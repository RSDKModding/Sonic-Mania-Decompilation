#ifndef OBJ_MAGPLATFORM_H
#define OBJ_MAGPLATFORM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMagPlatform;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMagPlatform;

// Object Struct
extern ObjectMagPlatform *MagPlatform;

// Standard Entity Events
void MagPlatform_Update();
void MagPlatform_LateUpdate();
void MagPlatform_StaticUpdate();
void MagPlatform_Draw();
void MagPlatform_Create(void* data);
void MagPlatform_StageLoad();
void MagPlatform_EditorDraw();
void MagPlatform_EditorLoad();
void MagPlatform_Serialize();

// Extra Entity Functions


#endif //!OBJ_MAGPLATFORM_H
