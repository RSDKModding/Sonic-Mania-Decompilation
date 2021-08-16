#ifndef OBJ_MENUSETUP_H
#define OBJ_MENUSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    bool32 dword8;
    bool32 initialized;
    bool32 dword10;
    bool32 gameLoaded;
    bool32 field_18;
    int field_1C;
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
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    Entity *dialog;
#endif
} ObjectMenuSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(timedState);
    int timer;
    int timeOut;
    int field_68;
    int fadeTimer;
    int fadeColour;
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
void MenuSetup_EditorDraw(void);
void MenuSetup_EditorLoad(void);
void MenuSetup_Serialize(void);

// Extra Entity Functions
void MenuSetup_StartTransition(void (*callback)(void), int time);

#if !RETRO_USE_PLUS
//Setup
void MenuSetup_Initialize(void);
bool32 MenuSetup_InitUserdata(void);
void MenuSetup_Unknown3(void);
void MenuSetup_Unknown52(void);
void MenuSetup_Unknown7(void);
int MenuSetup_GetActiveMenu(void);
void MenuSetup_ChangeMenuTrack(void);
void MenuSetup_SetBGColours(void);
void MenuSetup_ChangeMenu(void);

//Main Menu
bool32 MenuSetup_ReturnToTitleOption(void);
int MenuSetup_StartReturnToTitle(void);
void MenuSetup_ReturnToTitle(void);
void MenuSetup_ExitGame(void);
void MenuSetup_ExitGame_Confirm(void);
void MenuSetup_ExitGame_CB(void);
void MenuSetup_Unknown13(void);

//Save Select
int MenuSetup_GetMedalMods(void);
void MenuSetup_OpenSaveSelectMenu(void);
void MenuSetup_SaveFileCB(int status);
void MenuSetup_StartNewSave(void);
void MenuSetup_CheckNoSaveSelected(void);
void MenuSetup_OpenSecretsMenu(void);
void MenuSetup_SaveSel_YPressCB(void);

//Time Attack
void MenuSetup_TA_OpenZoneList_Sonic(void);
void MenuSetup_TA_OpenZoneList_Tails(void);
void MenuSetup_TA_OpenZoneList_Knux(void);
void MenuSetup_TA_StartAttempt_Callback(void);
void MenuSetup_TA_StartAttempt(void);
void MenuSetup_TA_Unknown50(void);
bool32 MenuSetup_TA_Unknown51(void);
void MenuSetup_TA_Unknown52(void);
void MenuSetup_Leaderboard_Unknown(void);

//Comptetition
void MenuSetup_VS_OpenCompRules(void);
void MenuSetup_VS_OpenCompZones(void);

//Options
void MenuSetup_Options_LaunchManual(void);

//Extras
void MenuSetup_OpenExtrasMenu(void);
void MenuSetup_Extras_ProcessButtonCB(void);
void MenuSetup_Extras_Start_Puyo_vsAI(void);
void MenuSetup_Extras_Callback_Puyo_vsAI(void);
void MenuSetup_Extras_Start_Puyo_vs2P(void);
void MenuSetup_Extras_Callback_Puyo_vs2P(void);
void MenuSetup_Extras_Start_Credits(void);
void MenuSetup_Extras_Callback_Credits(void);
void MenuSetup_Extras_StartDAGarden(void);
void MenuSetup_Extras_Callback_DAGarden(void);
void MenuSetup_Extras_Start_BSS_3K(void);
void MenuSetup_Extras_Callback_BSS_3K(void);
void MenuSetup_Extras_Start_BSS_Mania(void);
void MenuSetup_Extras_Callback_BSS_Mania(void);
#endif

#endif //!OBJ_MENUSETUP_H
