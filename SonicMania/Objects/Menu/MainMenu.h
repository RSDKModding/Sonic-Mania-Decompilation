#ifndef OBJ_MAINMENU_H
#define OBJ_MAINMENU_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectMainMenu {
    RSDK_OBJECT
    EntityUIControl *menuControl;
    EntityUIButtonPrompt *confirmPrompt;
    EntityUIDiorama *diorama;
};

// Entity Class
struct EntityMainMenu {
    RSDK_ENTITY
};

// Object Struct
extern ObjectMainMenu *MainMenu;

// Standard Entity Events
void MainMenu_Update(void);
void MainMenu_LateUpdate(void);
void MainMenu_StaticUpdate(void);
void MainMenu_Draw(void);
void MainMenu_Create(void *data);
void MainMenu_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MainMenu_EditorDraw(void);
void MainMenu_EditorLoad(void);
#endif
void MainMenu_Serialize(void);

// Extra Entity Functions
void MainMenu_Initialize(void);
bool32 MainMenu_BackPressCB_ReturnToTitle(void);
void MainMenu_ExitGame(void);
void MainMenu_ExitButton_ActionCB(void);
void MainMenu_StartExitGame(void);
void MainMenu_MenuButton_ActionCB(void);

#if MANIA_USE_EGS
void MainMenu_BuyPlusDialogCB(void);
#endif

void MainMenu_HandleUnlocks(void);
void MainMenu_SetupActions(void);
void MainMenu_MenuSetupCB(void);
#endif

#endif //! OBJ_MAINMENU_H
