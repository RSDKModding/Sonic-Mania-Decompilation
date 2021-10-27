#ifndef OBJ_GREENSCREEN_H
#define OBJ_GREENSCREEN_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectGreenScreen;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityGreenScreen;

// Object Struct
extern ObjectGreenScreen *GreenScreen;

// Standard Entity Events
void GreenScreen_Update(void);
void GreenScreen_LateUpdate(void);
void GreenScreen_StaticUpdate(void);
void GreenScreen_Draw(void);
void GreenScreen_Create(void* data);
void GreenScreen_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void GreenScreen_EditorDraw(void);
void GreenScreen_EditorLoad(void);
#endif
void GreenScreen_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_GREENSCREEN_H
