#ifndef OBJ_MMZLIGHTNING_H
#define OBJ_MMZLIGHTNING_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMMZLightning;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMMZLightning;

// Object Struct
extern ObjectMMZLightning *MMZLightning;

// Standard Entity Events
void MMZLightning_Update();
void MMZLightning_LateUpdate();
void MMZLightning_StaticUpdate();
void MMZLightning_Draw();
void MMZLightning_Create(void* data);
void MMZLightning_StageLoad();
void MMZLightning_EditorDraw();
void MMZLightning_EditorLoad();
void MMZLightning_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_MMZLIGHTNING_H
