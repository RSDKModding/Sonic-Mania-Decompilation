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
void MMZLightning_Update(void);
void MMZLightning_LateUpdate(void);
void MMZLightning_StaticUpdate(void);
void MMZLightning_Draw(void);
void MMZLightning_Create(void* data);
void MMZLightning_StageLoad(void);
void MMZLightning_EditorDraw(void);
void MMZLightning_EditorLoad(void);
void MMZLightning_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_MMZLIGHTNING_H
