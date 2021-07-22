#ifndef OBJ_OPTIONSMENU_H
#define OBJ_OPTIONSMENU_H

#include "SonicMania.h"

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
void OptionsMenu_Update(void);
void OptionsMenu_LateUpdate(void);
void OptionsMenu_StaticUpdate(void);
void OptionsMenu_Draw(void);
void OptionsMenu_Create(void* data);
void OptionsMenu_StageLoad(void);
void OptionsMenu_EditorDraw(void);
void OptionsMenu_EditorLoad(void);
void OptionsMenu_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_OPTIONSMENU_H
