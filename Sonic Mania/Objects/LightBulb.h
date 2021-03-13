#ifndef OBJ_LIGHTBULB_H
#define OBJ_LIGHTBULB_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLightBulb;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLightBulb;

// Object Struct
extern ObjectLightBulb *LightBulb;

// Standard Entity Events
void LightBulb_Update();
void LightBulb_LateUpdate();
void LightBulb_StaticUpdate();
void LightBulb_Draw();
void LightBulb_Create(void* data);
void LightBulb_StageLoad();
void LightBulb_EditorDraw();
void LightBulb_EditorLoad();
void LightBulb_Serialize();

// Extra Entity Functions


#endif //!OBJ_LIGHTBULB_H
