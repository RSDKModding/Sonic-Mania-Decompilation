#ifndef OBJ_MAGPLATFORM_H
#define OBJ_MAGPLATFORM_H

#include "SonicMania.h"

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
void MagPlatform_Update(void);
void MagPlatform_LateUpdate(void);
void MagPlatform_StaticUpdate(void);
void MagPlatform_Draw(void);
void MagPlatform_Create(void* data);
void MagPlatform_StageLoad(void);
void MagPlatform_EditorDraw(void);
void MagPlatform_EditorLoad(void);
void MagPlatform_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MAGPLATFORM_H
