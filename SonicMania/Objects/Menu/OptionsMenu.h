#ifndef OBJ_OPTIONSMENU_H
#define OBJ_OPTIONSMENU_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectOptionsMenu {
    RSDK_OBJECT
    EntityUIControl *optionsControl;
    EntityUIButtonPrompt *helpPrompt;
    EntityUIControl *videoControl;
    EntityUIControl *videoControl_Windows;
    EntityUIControl *soundControl;
    EntityUIControl *dataOptionsControl;
    EntityUIControl *controlsControl_Windows;
    EntityUIControl *controlsControl_KB;
    EntityUIControl *controlsControl_PS4;
    EntityUIControl *controlsControl_XB1;
    EntityUIControl *controlsControl_NX;
    EntityUIControl *controlsControl_NXGrip;
    EntityUIControl *controlsControl_NXJoycon;
    EntityUIControl *controlsControl_NXPro;
    EntityUIControl *languageControl;
    EntityUIControl *languageControl_Legacy;
    EntityUIDiorama *diorama;
    EntityUIInfoLabel *selectDataToEraseLabel;
};

// Entity Class
struct EntityOptionsMenu {
    RSDK_ENTITY
};

// Object Struct
extern ObjectOptionsMenu *OptionsMenu;

// Standard Entity Events
void OptionsMenu_Update(void);
void OptionsMenu_LateUpdate(void);
void OptionsMenu_StaticUpdate(void);
void OptionsMenu_Draw(void);
void OptionsMenu_Create(void *data);
void OptionsMenu_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void OptionsMenu_EditorDraw(void);
void OptionsMenu_EditorLoad(void);
#endif
void OptionsMenu_Serialize(void);

// Extra Entity Functions
void OptionsMenu_Initialize(void);
void OptionsMenu_HandleUnlocks(void);
void OptionsMenu_SetupActions(void);
void OptionsMenu_HandleMenuReturn(void);
void OptionsMenu_InitVideoOptionsMenu(void);
void OptionsMenu_VideoControl_Win_MenuUpdateCB(void);
void OptionsMenu_VideoControl_Win_YPressCB(void);
void OptionsMenu_DlgRunnerCB_RevertVideoChanges(void);
bool32 OptionsMenu_VideoControl_Win_BackPressCB(void);
void OptionsMenu_ApplyChangesDlg_BackPress_YesCB(void);
void OptionsMenu_ApplyChangesDlg_Win_NoCB(void);
void OptionsMenu_ApplyChangesDlg_NoCB(void);
void OptionsMenu_ApplyChangesDlg_Win_YesCB(void);
void OptionsMenu_ApplyChangesDlg_BackPress_NoCB(void);
void OptionsMenu_VideoMenuButton_ActionCB(void);
void OptionsMenu_SoundMenuButton_ActionCB(void);
void OptionsMenu_LanguageMenuButton_ActionCB(void);
void OptionsMenu_DataOptionsMenuButton_ActionCB(void);
void OptionsMenu_ControlsMenuButton_DefaultKB_ActionCB(void);
void OptionsMenu_ControlsMenuButton_Default_ActionCB(void);
void OptionsMenu_ControlsMenuButton_ActionCB(void);
void OptionsMenu_SetDefaultMappings(void);
void OptionsMenu_SetupKBControlsMenu(int32 playerID);
void OptionsMenu_KeyboardIDButton_Win_ActionCB(void);
void OptionsMenu_MenuSetupCB(void);
void OptionsMenu_SaveOptionsCB_Load(bool32 success);
void OptionsMenu_TransitionCB_ReloadScene(void);
void OptionsMenu_SaveOptionsCB_Action(bool32 success);
void OptionsMenu_LanguageButton_ActionCB(void);
void OptionsMenu_ShaderButton_ActionCB(void);
void OptionsMenu_WindowScaleButton_ActionCB(void);
void OptionsMenu_BorderlessButton_ActionCB(void);
void OptionsMenu_FullScreenButton_ActionCB(void);
void OptionsMenu_VSyncButton_ActionCB(void);
void OptionsMenu_TripleBufferButton_ActionCB(void);
void OptionsMenu_UISlider_ChangedCB(void);
void OptionsMenu_ShowManual(void);
void OptionsMenu_EraseSaveDataCB(bool32 success);
void OptionsMenu_AreYouSureDlg_YesCB_EraseSaveGame(void);
void OptionsMenu_AreYouSureDlg_YesCB_EraseAllData(void);
void OptionsMenu_EraseSaveGameButton_ActionCB(void);
void OptionsMenu_AreYouSureDlg_YesCB_EraseMedallions(void);
void OptionsMenu_EraseMedallionsButton_ActionCB(void);
void OptionsMenu_AreYouSureDlg_YesCB_EraseTimeAttack(void);
void OptionsMenu_EraseTimeAttackButton_ActionCB(void);
void OptionsMenu_AreYouSureDlg_YesCB_EraseReplays(void);
void OptionsMenu_EraseReplaysCB(bool32 success);
void OptionsMenu_EraseReplaysButton_ActionCB(void);
void OptionsMenu_EraseAllButton_ActionCB(void);

#endif

#endif //! OBJ_OPTIONSMENU_H
