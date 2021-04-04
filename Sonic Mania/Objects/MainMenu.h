#ifndef OBJ_MAINMENU_H
#define OBJ_MAINMENU_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMainMenu;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMainMenu;

// Object Struct
extern ObjectMainMenu *MainMenu;

// Standard Entity Events
void MainMenu_Update(void);
void MainMenu_LateUpdate(void);
void MainMenu_StaticUpdate(void);
void MainMenu_Draw(void);
void MainMenu_Create(void* data);
void MainMenu_StageLoad(void);
void MainMenu_EditorDraw(void);
void MainMenu_EditorLoad(void);
void MainMenu_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_MAINMENU_H
