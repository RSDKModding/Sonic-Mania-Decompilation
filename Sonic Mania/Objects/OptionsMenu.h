#ifndef OBJ_OPTIONSMENU_H
#define OBJ_OPTIONSMENU_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectOptionsMenu;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityOptionsMenu;

// Object Struct
extern ObjectOptionsMenu *OptionsMenu;

// Standard Entity Events
void OptionsMenu_Update();
void OptionsMenu_LateUpdate();
void OptionsMenu_StaticUpdate();
void OptionsMenu_Draw();
void OptionsMenu_Create(void* data);
void OptionsMenu_StageLoad();
void OptionsMenu_EditorDraw();
void OptionsMenu_EditorLoad();
void OptionsMenu_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_OPTIONSMENU_H
