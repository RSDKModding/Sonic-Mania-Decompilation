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
void MainMenu_Update();
void MainMenu_LateUpdate();
void MainMenu_StaticUpdate();
void MainMenu_Draw();
void MainMenu_Create(void* data);
void MainMenu_StageLoad();
void MainMenu_EditorDraw();
void MainMenu_EditorLoad();
void MainMenu_Serialize();

// Extra Entity Functions
#endif

#endif //!OBJ_MAINMENU_H
