#ifndef OBJ_LIGHTBULB_H
#define OBJ_LIGHTBULB_H

#include "SonicMania.h"

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
void LightBulb_Update(void);
void LightBulb_LateUpdate(void);
void LightBulb_StaticUpdate(void);
void LightBulb_Draw(void);
void LightBulb_Create(void* data);
void LightBulb_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LightBulb_EditorDraw(void);
void LightBulb_EditorLoad(void);
#endif
void LightBulb_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LIGHTBULB_H
