#ifndef OBJ_MENUSETUP_H
#define OBJ_MENUSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    bool32 dword8;
    bool32 initialized;
    bool32 dword10;
    bool32 gameLoaded;
    bool32 field_18;
    int32 field_1C;
    EntityFXFade *fxFade;
#if !RETRO_USE_PLUS
    Entity *saveSelPrompt;
    Entity *mainMenu;
    Entity *timeAttack;
    Entity *timeAttackZones;
    Entity *leaderboards;
    Entity *leaderboardWidget;
    Entity *leaderboardPrompt;
    Entity *competition;
    Entity *competitionRules;
    Entity *competitionZones;
    Entity *competitionRound;
    Entity *roundLabel;
    Entity *roundScoreboard;
    Entity *competitionTotal;
    Entity *totalLabel;
    Entity *totalScoreboard;
    Entity *saveSelect;
    Entity *noSaveMode;
    Entity *secrets;
    Entity *extras;
    Entity *options;
    Entity *optionsPrompt;
    Entity *video;
    Entity *video_win;
    Entity *sound;
    Entity *controls_win;
    Entity *controls_KB;
    Entity *controls_PS4;
    Entity *controls_XB1;
    Entity *controls_NX;
    Entity *controls_NX_Grip;
    Entity *controls_NX_JoyCon;
    Entity *controls_NX_Pro;
    Entity *language;
    int32 field_138;
    int32 field_13C;
    int32 field_140;
    int32 field_144;
    Entity *dialog;
#endif
} ObjectMenuSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDelay);
    int32 timer;
    int32 delay;
    int32 field_68;
    int32 fadeTimer;
    int32 fadeColour;
} EntityMenuSetup;

// Object Struct
extern ObjectMenuSetup *MenuSetup;

// Standard Entity Events
void MenuSetup_Update(void);
void MenuSetup_LateUpdate(void);
void MenuSetup_StaticUpdate(void);
void MenuSetup_Draw(void);
void MenuSetup_Create(void* data);
void MenuSetup_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MenuSetup_EditorDraw(void);
void MenuSetup_EditorLoad(void);
#endif
void MenuSetup_Serialize(void);

// Extra Entity Functions
void MenuSetup_StartTransition(void (*callback)(void), int32 delay);

#if !RETRO_USE_PLUS
//Setup
void MenuSetup_Initialize(void);
bool32 MenuSetup_InitUserdata(void);
void MenuSetup_Unknown3(void);
void MenuSetup_Unknown52(void);
void MenuSetup_HandleMenuReturn(void);
int32 MenuSetup_GetActiveMenu(void);
void MenuSetup_ChangeMenuTrack(void);
void MenuSetup_SetBGColours(void);
void MenuSetup_MenuButton_ActionCB(void);
void MenuSetup_StartTransitionLB(void (*callback)(void), int32 delay);

//Main Menu
bool32 MenuSetup_ReturnToTitleOption(void);
int32 MenuSetup_StartReturnToTitle(void);
void MenuSetup_ReturnToTitle(void);
void MenuSetup_ExitGame(void);
void MenuSetup_ExitGame_ActionCB(void);
void MenuSetup_ExitGame_CB(void);
void MenuSetup_State_HandleTransition(void);

//Save Select
int32 MenuSetup_GetMedalMods(void);
void MenuSetup_OpenSaveSelectMenu(void);
void MenuSetup_SaveFileCB(int32 status);
void MenuSetup_SaveSlot_ActionCB(void);
void MenuSetup_SaveSel_MenuUpdateCB(void);
void MenuSetup_OpenSecretsMenu(void);
void MenuSetup_SaveSel_YPressCB(void);

//Time Attack
void MenuSetup_TA_OpenZoneList_Sonic(void);
void MenuSetup_TA_OpenZoneList_Tails(void);
void MenuSetup_TA_OpenZoneList_Knux(void);
void MenuSetup_TA_TAZoneModule_ActionCB(void);
void MenuSetup_TA_StartAttempt(void);
void MenuSetup_TA_Unknown50(void);
bool32 MenuSetup_TA_Unknown51(void);
void MenuSetup_TA_Unknown52(void);
void MenuSetup_Leaderboard_Unknown(void);

//Comptetition
void MenuSetup_VS_OpenCompRules(void);
void MenuSetup_VS_OpenCompZones(void);
void MenuSetup_VS_ProcessButtonCB(void);
void MenuSetup_VS_MenuSetupCB(void);
void MenuSetup_VS_StartMatch(void);
void MenuSetup_VS_StartMatch_ActionCB(void);
void MenuSetup_VS_Unknown52(void);
void MenuSetup_VS_OpenCompTotal(void);
void MenuSetup_VS_Round_ProcessButtonCB(void);
void MenuSetup_VS_Round_MenuSetupCB(void);
void MenuSetup_VS_OpenCompetition(void);
void MenuSetup_VS_Total_ProcessButtonCB(void);
void MenuSetup_VS_Total_MenuSetupCB(void);
void MenuSetup_VS_Total_MenuUpdateCB(void);
void MenuSetup_VS_Unknown58(void);
void MenuSetup_VS_BackoutFromVsCharSelect_CB(void);
bool32 MenuSetup_VS_BackoutFromVsCharSelect(void);
void MenuSetup_VS_Unknown59(void);
bool32 MenuSetup_VS_Unknown60(void);
void MenuSetup_VS_StartPuyoMatch(void);

// Options
void MenuSetup_Options_OpenVideoMenu(void);
void MenuSetup_Options_OpenSoundMenu(void);
void MenuSetup_Options_OpenLanguageMenu(void);
void MenuSetup_Options_OpenControlsMenu(void);
void MenuSetup_Options_Unknown22_P1(void);
void MenuSetup_Options_Unknown22_P2(void);
void MenuSetup_Options_Unknown51(int32 id);
void MenuSetup_OptionsVideo_Win_MenuUpdateCB(void);
void MenuSetup_Unknown54(void);
void MenuSetup_Options_OpenKBControlsMenu(void);
void MenuSetup_Options_MenuSetupCB(void);
void MenuSetup_Options_Unknown27(int32 ststaus);
void MenuSetup_Options_LaunchManual(void);
void MenuSetup_Options_Unknown52(void);
void MenuSetup_Options_ShaderIDChanged_CB(void);
void MenuSetup_Options_WinSizeChanged_CB(void);
void MenuSetup_Options_BorderedChanged_CB(void);
void MenuSetup_Options_FullscreenChanged_CB(void);
void MenuSetup_Options_VSyncChanged_CB(void);
void MenuSetup_Options_TripleBufferedChanged_CB(void);
void MenuSetup_Options_SliderChanged_CB(void);

//Extras
void MenuSetup_OpenExtrasMenu_ActionCB(void);
void MenuSetup_Extras_ProcessButtonCB(void);
void MenuSetup_Extras_Start_Puyo_vsAI(void);
void MenuSetup_Extras_Callback_Puyo_vsAI(void);
void MenuSetup_Extras_Start_Puyo_vs2P(void);
void MenuSetup_Extras_Callback_Puyo_vs2P(void);
void MenuSetup_Extras_Start_Credits(void);
void MenuSetup_Extras_Credits_ActionCB(void);
void MenuSetup_Extras_StartDAGarden(void);
void MenuSetup_Extras_DAGarden_ActionCB(void);
void MenuSetup_Extras_Start_BSS_3K(void);
void MenuSetup_Extras_Callback_BSS_3K(void);
void MenuSetup_Extras_Start_BSS_Mania(void);
void MenuSetup_Extras_Callback_BSS_Mania(void);
#endif

#endif //!OBJ_MENUSETUP_H
