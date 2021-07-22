#ifndef OBJ_MAINMENU_H
#define OBJ_MAINMENU_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    Entity *menuControlPtr;
    Entity *promptPtr;
    Entity *dioramaPtr;
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
void MainMenu_Initialize(void);
int MainMenu_ReturnToTitleOption(void);
int MainMenu_ExitGame(void);
void MainMenu_ExitGameOption(void);
int MainMenu_ReturnToTitleOption(void);
int MainMenu_StartExitGame(void);
void MainMenu_ChangeMenu(void);

#if RETRO_USE_EGS
int MainMenu_BuyPlusDialogCB(void)
#endif

void MainMenu_Unknown2(void);
void MainMenu_Unknown3(void);
void MainMenu_Unknown4(void);
#endif

#endif //!OBJ_MAINMENU_H
