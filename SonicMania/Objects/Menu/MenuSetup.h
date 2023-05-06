#ifndef OBJ_MENUSETUP_H
#define OBJ_MENUSETUP_H

#include "Game.h"

typedef enum {
    MAINMENU_MAIN,
    MAINMENU_TIMEATTACK,
    MAINMENU_COMPETITION,
    MAINMENU_SAVESELECT,
#if MANIA_USE_PLUS
    MAINMENU_SAVESELECT_ENCORE,
#endif
} MainMenuIDs;

// Object Class
struct ObjectMenuSetup {
    RSDK_OBJECT
    int32 unused1;
    bool32 initializedMenuReturn;
    bool32 initializedMenu;
    bool32 initializedAPI;
    bool32 initializedSaves;
    int32 unused2;
    int32 unused3;
    EntityFXFade *fxFade;
#if !MANIA_USE_PLUS
    EntityUIButtonPrompt *delSavePrompt;
    EntityUIControl *mainMenu;
    EntityUIControl *timeAttack;
    EntityUIControl *timeAttackZones;
    EntityUIControl *leaderboards;
    EntityUILeaderboard *leaderboardWidget;
    EntityUIButtonPrompt *leaderboardPrompt;
    EntityUIControl *competition;
    EntityUIControl *competitionRules;
    EntityUIControl *competitionZones;
    EntityUIControl *competitionRound;
    EntityUIInfoLabel *roundLabel;
    EntityUIVsScoreboard *roundScoreboard;
    EntityUIControl *competitionTotal;
    EntityUIInfoLabel *totalLabel;
    EntityUIVsScoreboard *totalScoreboard;
    EntityUIControl *saveSelect;
    EntityUIControl *noSaveMode;
    EntityUIControl *secrets;
    EntityUIControl *extras;
    EntityUIControl *options;
    EntityUIButtonPrompt *optionsPrompt;
    EntityUIControl *video;
    EntityUIControl *video_win;
    EntityUIControl *sound;
    EntityUIControl *controls_win;
    EntityUIControl *controls_KB;
    EntityUIControl *controls_PS4;
    EntityUIControl *controls_XB1;
    EntityUIControl *controls_NX;
    EntityUIControl *controls_NX_Grip;
    EntityUIControl *controls_NX_JoyCon;
    EntityUIControl *controls_NX_Pro;
    EntityUIControl *language;
    int32 unused4;
    int32 unused5;
    int32 vsTotalTimer;
    int32 unused6;
    EntityUIDialog *connectingDlg;
#endif
};

// Entity Class
struct EntityMenuSetup {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(callback);
    int32 timer;
    int32 delay;
    int32 fadeShift;
    int32 fadeTimer;
    int32 fadeColor;
};

// Object Struct
extern ObjectMenuSetup *MenuSetup;

// Standard Entity Events
void MenuSetup_Update(void);
void MenuSetup_LateUpdate(void);
void MenuSetup_StaticUpdate(void);
void MenuSetup_Draw(void);
void MenuSetup_Create(void *data);
void MenuSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MenuSetup_EditorDraw(void);
void MenuSetup_EditorLoad(void);
#endif
void MenuSetup_Serialize(void);

// Extra Entity Functions
void MenuSetup_StartTransition(void (*callback)(void), int32 delay);

#if !MANIA_USE_PLUS
// Setup
void MenuSetup_Initialize(void);
bool32 MenuSetup_InitAPI(void);
void MenuSetup_SetupActions(void);
void MenuSetup_HandleUnlocks(void);
void MenuSetup_HandleMenuReturn(void);
int32 MenuSetup_GetActiveMenu(void);
void MenuSetup_ChangeMenuTrack(void);
void MenuSetup_SetBGColors(void);
void MenuSetup_MenuButton_ActionCB(void);
void MenuSetup_StartTransitionLB(void (*callback)(void), int32 delay);

// Main Menu
bool32 MenuSetup_BackPressCB_ReturnToTitle(void);
void MenuSetup_StartReturnToTitle(void);
void MenuSetup_ReturnToTitle(void);
void MenuSetup_ExitGame(void);
void MenuSetup_ExitGame_ActionCB(void);
void MenuSetup_ExitGame_CB(void);
void MenuSetup_State_HandleTransition(void);

// Save Select
int32 MenuSetup_GetMedalMods(void);
void MenuSetup_OpenSaveSelectMenu(void);
#if MANIA_USE_PLUS
void MenuSetup_SaveFileCB(bool32 success);
#else
void MenuSetup_SaveFileCB(void);
#endif
void MenuSetup_SaveSlot_ActionCB(void);
void MenuSetup_SaveSel_MenuUpdateCB(void);
void MenuSetup_OpenSecretsMenu(void);
void MenuSetup_SaveSel_YPressCB(void);

// Time Attack
void MenuSetup_TA_OpenZoneList_Sonic(void);
void MenuSetup_TA_OpenZoneList_Tails(void);
void MenuSetup_TA_OpenZoneList_Knux(void);
void MenuSetup_TA_TAZoneModule_ActionCB(void);
void MenuSetup_TA_StartAttempt(void);
void MenuSetup_TA_Leaderboards_TransitionCB(void);
bool32 MenuSetup_TA_Leaderboards_BackPressCB(void);
void MenuSetup_TA_Leaderboards_YPressCB(void);
void MenuSetup_State_SetupLeaderboards(void);

// Comptetition
void MenuSetup_VS_OpenCompRules(void);
void MenuSetup_VS_OpenCompZones(void);
void MenuSetup_VS_ProcessButtonCB(void);
void MenuSetup_VS_MenuSetupCB(void);
void MenuSetup_VS_StartMatch(void);
void MenuSetup_VS_StartMatch_ActionCB(void);
void MenuSetup_VS_RulesButton_ActionCB(void);
void MenuSetup_VS_OpenCompTotal(void);
void MenuSetup_VS_Round_ProcessButtonCB(void);
void MenuSetup_VS_Round_MenuSetupCB(void);
void MenuSetup_VS_GotoCompetition(void);
void MenuSetup_VS_Total_ProcessButtonCB(void);
void MenuSetup_VS_Total_MenuSetupCB(void);
void MenuSetup_VS_Total_MenuUpdateCB(void);
void MenuSetup_VS_ExitComp_TransitionCB(void);
void MenuSetup_VS_BackoutFromVsCharSelect_CB(void);
bool32 MenuSetup_VS_BackoutFromVsCharSelect(void);
void MenuSetup_VS_ExitComp_YesCB(void);
bool32 MenuSetup_VS_CompZones_BackPressCB(void);
void MenuSetup_VS_StartPuyoMatch(void);

// Options
void MenuSetup_Options_VideoMenuButton_ActionCB(void);
void MenuSetup_Options_SoundMenuButton_ActionCB(void);
void MenuSetup_Options_LanguageMenuButton_ActionCB(void);
void MenuSetup_Options_ControlsMenuButton_ActionCB(void);
void MenuSetup_Options_SetDefaultMappings_P1(void);
void MenuSetup_Options_SetDefaultMappings_P2(void);
void MenuSetup_Options_SetupKBControlsMenu(int32 playerID);
void MenuSetup_OptionsVideo_Win_MenuUpdateCB(void);
void MenuSetup_OptionsVideo_Win_InitVideoOptionsMenu(void);
void MenuSetup_Options_OpenKBControlsMenu(void);
void MenuSetup_Options_MenuSetupCB(void);
void MenuSetup_Options_SaveOptionsCB_Load(void);
void MenuSetup_Options_LaunchManual(void);
void MenuSetup_OptionsLanguage_LanguageButton_ActionCB(void);
void MenuSetup_OptionsVideo_ShaderButton_ActionCB(void);
void MenuSetup_OptionsVideo_WindowScaleButton_ActionCB(void);
void MenuSetup_OptionsVideo_BorderlessButton_ActionCB(void);
void MenuSetup_OptionsVideo_FullscreenButton_ActionCB(void);
void MenuSetup_OptionsVideo_VSyncButton_ActionCB(void);
void MenuSetup_OptionsVideo_TripleBufferButton_ActionCB(void);
void MenuSetup_OptionsVideo_UISlider_ChangedCB(void);

// Extras
void MenuSetup_OpenExtrasMenu_ActionCB(void);
void MenuSetup_Extras_ProcessButtonCB(void);
void MenuSetup_Extras_Start_Puyo_vsAI(void);
void MenuSetup_Extras_Puyo_vsAI_ActionCB(void);
void MenuSetup_Extras_Start_Puyo_vs2P(void);
void MenuSetup_Extras_Puyo_vs2P_ActionCB(void);
void MenuSetup_Extras_Start_Credits(void);
void MenuSetup_Extras_Credits_ActionCB(void);
void MenuSetup_Extras_StartDAGarden(void);
void MenuSetup_Extras_DAGarden_ActionCB(void);
void MenuSetup_Extras_Start_BSS_3K(void);
void MenuSetup_Extras_BSS_3K_ActionCB(void);
void MenuSetup_Extras_Start_BSS_Mania(void);
void MenuSetup_Extras_BSS_Mania_ActionCB(void);
#endif

#endif //! OBJ_MENUSETUP_H
