#ifndef OBJ_TIMEATTACKMENU_H
#define OBJ_TIMEATTACKMENU_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    Entity *timeAttackControl;
    Entity *timeAttackControl_Legacy;
    Entity *taZoneSelControl;
    Entity *prompt1;
    Entity *taDetailsControl;
    Entity *banner;
    Entity *leaderboardsControl;
    Entity *leaderboard;
    Entity *prompt2;
    Entity *banner2;
    Entity *replaysControl;
    Entity *replayCarousel;
    Entity *replayPrompt;
    Entity *connectingDlg;
    bool32 encoreMode;
    bool32 field_40;
    bool32 field_44;
} ObjectTimeAttackMenu;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(callback);
    int timeOut;
} EntityTimeAttackMenu;

// Object Struct
extern ObjectTimeAttackMenu *TimeAttackMenu;

// Standard Entity Events
void TimeAttackMenu_Update(void);
void TimeAttackMenu_LateUpdate(void);
void TimeAttackMenu_StaticUpdate(void);
void TimeAttackMenu_Draw(void);
void TimeAttackMenu_Create(void *data);
void TimeAttackMenu_StageLoad(void);
void TimeAttackMenu_EditorDraw(void);
void TimeAttackMenu_EditorLoad(void);
void TimeAttackMenu_Serialize(void);

// Extra Entity Functions
void TimeAttackMenu_Initialize(void);
void TimeAttackMenu_Unknown2(void);
void TimeAttackMenu_Unknown3(void);
void TimeAttackMenu_Unknown4(void);
void TimeAttackMenu_SetEncoreLayouts(bool32 enabled);
void TimeAttackMenu_XPressCB_Replay(void);
void TimeAttackMenu_Unknown7(void);
void TimeAttackMenu_Unknown8(void);
void TimeAttackMenu_UnknownCB4_LB(void);
void TimeAttackMenu_Unknown10(signed int zone, int playerID, signed int act, int encore, int a5, void (*callback)(void));
void TimeAttackMenu_Options2CB_ReplayCarousel(void);
void TimeAttackMenu_AddReplayEntry(int a1, int a2);
void TimeAttackMenu_ReplayLoad_CB(bool32 a1);
void TimeAttackMenu_Unknown13(void);
void TimeAttackMenu_Unknown14(void);
void TimeAttackMenu_Unknown15(void);
void TimeAttackMenu_Unknown16(void);
void TimeAttackMenu_Unknown17(void);
void TimeAttackMenu_UnknownCB3_Replay(void);
void TimeAttackMenu_UnknownCB4_Replay(void);
void TimeAttackMenu_Unknown20(void);
void TimeAttackMenu_Unknown21(void);
void TimeAttackMenu_YPressCB_Replay(void);
void TimeAttackMenu_UnknownCB1_ReplayCarousel(void);
void TimeAttackMenu_Unknown24(void);
void TimeAttackMenu_TAModuleCB(void);
void TimeAttackMenu_Unknown26(void);
void TimeAttackMenu_Unknown27(void);
void TimeAttackMenu_YPressCB_ZoneSel(void);
bool32 TimeAttackMenu_BackPressCB_ZoneSel(void);
void TimeAttackMenu_YPressCB_Details(void);
void TimeAttackMenu_ResetTimes_YesCB(void);
void TimeAttackMenu_XPressCB_Details(void);
void TimeAttackMenu_Options2CB_Replays(void);
void TimeAttackMenu_Unknown34(void);
void TimeAttackMenu_UnknownCB3_Details(void);
void TimeAttackMenu_UnknownCB1_Replays(void);
void TimeAttackMenu_Options2CB_CharButton(void);
void TimeAttackMenu_Unknown38(void);
bool32 TimeAttackMenu_LeaderboardsBackPressCB(void);
void TimeAttackMenu_YPressCB_LB(void);
void TimeAttackMenu_Unknown41(void);
void TimeAttackMenu_Unknown42(void *c);

#endif

#endif //! OBJ_TIMEATTACKMENU_H
