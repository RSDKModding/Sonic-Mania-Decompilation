#ifndef OBJ_OPTIONSMENU_H
#define OBJ_OPTIONSMENU_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    Entity *optionsControl;
    Entity *prompt;
    Entity *videoControl;
    Entity *videoControl_Windows;
    Entity *soundControl;
    Entity *dataOptionsControl;
    Entity *controlsControl_Windows;
    Entity *controlsControl_KB;
    Entity *controlsControl_PS4;
    Entity *controlsControl_XB1;
    Entity *controlsControl_NX;
    Entity *controlsControl_NXGrip;
    Entity *controlsControl_NXJoycon;
    Entity *controlsControl_NXPro;
    Entity *languageControl;
    Entity *languageControl_Legacy;
    Entity *diorama;
    Entity *label;
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
#if RETRO_INCLUDE_EDITOR
void OptionsMenu_EditorDraw(void);
void OptionsMenu_EditorLoad(void);
#endif
void OptionsMenu_Serialize(void);

// Extra Entity Functions
void OptionsMenu_Initialize(void);
void OptionsMenu_HandleUnlocks(void);
void OptionsMenu_SetupActions(void);
void OptionsMenu_HandleMenuReturn(void);
void OptionsMenu_Unknown5(void);
void OptionsMenu_VideoControl_Win_MenuUpdateCB(void);
void OptionsMenu_VideoControl_Win_YPressCB(void);
void OptionsMenu_Unknown8(void);
bool32 OptionsMenu_VideoControl_Win_BackPressCB(void);
void OptionsMenu_Unknown10(void);
void OptionsMenu_Unknown11(void);
void OptionsMenu_Unknown12(void);
void OptionsMenu_Unknown13(void);
void OptionsMenu_Unknown14(void);
void OptionsMenu_Unknown15(void);
void OptionsMenu_Unknown16(void);
void OptionsMenu_Unknown17(void);
void OptionsMenu_Unknown18(void);
void OptionsMenu_TransitionToKeyboard_CB(void);
void OptionsMenu_TransitionToDefaultController_CB(void);
void OptionsMenu_Unknown21(void);
void OptionsMenu_SetDefaultMappings(void);
void OptionsMenu_Unknown23(int32 id);
void OptionsMenu_Unknown24(void);
void OptionsMenu_MenuSetupCB(void);
void OptionsMenu_Unknown27(int32 status);
void OptionsMenu_Unknown28(void);
void OptionsMenu_Unknown29(int32 status);
void OptionsMenu_Unknown30(void);
void OptionsMenu_Unknown31(void);
void OptionsMenu_Unknown32(void);
void OptionsMenu_Unknown33(void);
void OptionsMenu_Unknown34(void);
void OptionsMenu_Unknown35(void);
void OptionsMenu_Unknown36(void);
void OptionsMenu_UISlider_ChangedCB(void);
void OptionsMenu_ShowManual(void);
void OptionsMenu_EraseSaveDataCB(int32 status);
void OptionsMenu_EraseAllSaveData(void);
void OptionsMenu_EraseAllData(void);
void OptionsMenu_Unknown39(void);
void OptionsMenu_Unknown40(void);
void OptionsMenu_Unknown41(void);
void OptionsMenu_Unknown42(void);
void OptionsMenu_Unknown43(void);
void OptionsMenu_Unknown44(void);
void OptionsMenu_Unknown45(bool32 status);
void OptionsMenu_Unknown46(void);
void OptionsMenu_EraseAll_Confirm(void);

#endif

#endif //!OBJ_OPTIONSMENU_H
