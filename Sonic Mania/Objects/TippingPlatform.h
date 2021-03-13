#ifndef OBJ_TIPPINGPLATFORM_H
#define OBJ_TIPPINGPLATFORM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTippingPlatform;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTippingPlatform;

// Object Struct
extern ObjectTippingPlatform *TippingPlatform;

// Standard Entity Events
void TippingPlatform_Update();
void TippingPlatform_LateUpdate();
void TippingPlatform_StaticUpdate();
void TippingPlatform_Draw();
void TippingPlatform_Create(void* data);
void TippingPlatform_StageLoad();
void TippingPlatform_EditorDraw();
void TippingPlatform_EditorLoad();
void TippingPlatform_Serialize();

// Extra Entity Functions


#endif //!OBJ_TIPPINGPLATFORM_H
