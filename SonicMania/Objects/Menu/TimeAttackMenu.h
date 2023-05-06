#ifndef OBJ_TIMEATTACKMENU_H
#define OBJ_TIMEATTACKMENU_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectTimeAttackMenu {
    RSDK_OBJECT
    EntityUIControl *timeAttackControl;
    EntityUIControl *timeAttackControl_Legacy;
    EntityUIControl *taZoneSelControl;
    EntityUIButtonPrompt *switchModePrompt;
    EntityUIControl *taDetailsControl;
    EntityUITABanner *detailsBanner;
    EntityUIControl *leaderboardsControl;
    EntityUILeaderboard *leaderboard;
    EntityUIButtonPrompt *topRankPrompt;
    EntityUITABanner *leaderboardsBanner;
    EntityUIControl *replaysControl;
    EntityUIReplayCarousel *replayCarousel;
    EntityUIButtonPrompt *replayPrompt;
    EntityUIDialog *connectingDlg;
    bool32 encoreMode;
    bool32 prevIsUser;
    int32 isUser;
};

// Entity Class
struct EntityTimeAttackMenu {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(callback);
    int32 delay;
};

// Object Struct
extern ObjectTimeAttackMenu *TimeAttackMenu;

// Standard Entity Events
void TimeAttackMenu_Update(void);
void TimeAttackMenu_LateUpdate(void);
void TimeAttackMenu_StaticUpdate(void);
void TimeAttackMenu_Draw(void);
void TimeAttackMenu_Create(void *data);
void TimeAttackMenu_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TimeAttackMenu_EditorDraw(void);
void TimeAttackMenu_EditorLoad(void);
#endif
void TimeAttackMenu_Serialize(void);

// Extra Entity Functions
void TimeAttackMenu_Initialize(void);
void TimeAttackMenu_HandleUnlocks(void);
void TimeAttackMenu_SetupActions(void);
void TimeAttackMenu_HandleMenuReturn(void);
void TimeAttackMenu_SetEncoreLayouts(bool32 enabled);
void TimeAttackMenu_DeleteReplayActionCB(void);
void TimeAttackMenu_ConfirmDeleteReplay_Yes_CB(void);
void TimeAttackMenu_DeleteReplayCB(bool32 success);
void TimeAttackMenu_MenuUpdateCB_LB(void);
void TimeAttackMenu_SetupLeaderboards(int32 zoneID, int32 characterID, int32 act, bool32 isEncore, bool32 isUser, void (*callback)(void));
void TimeAttackMenu_ReplayCarousel_ActionCB(void);
void TimeAttackMenu_WatchReplay(int32 row, bool32 showGhost);
void TimeAttackMenu_ReplayLoad_CB(bool32 success);
void TimeAttackMenu_WatchReplayActionCB_ReplaysMenu(void);
void TimeAttackMenu_ChallengeReplayActionCB_ReplaysMenu(void);
void TimeAttackMenu_WatchReplayCB_RanksMenu(void);
void TimeAttackMenu_ChallengeReplayCB_RanksMenu(void);
void TimeAttackMenu_LoadScene_Fadeout(void);
void TimeAttackMenu_MenuSetupCB_Replay(void);
void TimeAttackMenu_MenuUpdateCB_Replay(void);
void TimeAttackMenu_MenuUpdateCB(void);
void TimeAttackMenu_ReplayButton_ActionCB(void);
void TimeAttackMenu_YPressCB_Replay(void);
void TimeAttackMenu_SortReplayChoiceCB(void);
void TimeAttackMenu_SetupDetailsView(void);
void TimeAttackMenu_TAModule_ActionCB(void);
void TimeAttackMenu_StartTAAttempt(void);
void TimeAttackMenu_LoadScene(void);
void TimeAttackMenu_YPressCB_ZoneSel(void);
bool32 TimeAttackMenu_BackPressCB_ZoneSel(void);
void TimeAttackMenu_YPressCB_Details(void);
void TimeAttackMenu_ResetTimes_YesCB(void);
void TimeAttackMenu_XPressCB_Details(void);
void TimeAttackMenu_TAZoneModule_ActionCB(void);
void TimeAttackMenu_RankButton_ActionCB(void);
void TimeAttackMenu_MenuSetupCB_Details(void);
void TimeAttackMenu_TAZoneModule_ChoiceChangeCB(void);
void TimeAttackMenu_CharButton_ActionCB(void);
void TimeAttackMenu_TransitionToDetailsCB(void);
bool32 TimeAttackMenu_LeaderboardsBackPressCB(void);
void TimeAttackMenu_YPressCB_LB(void);
void TimeAttackMenu_State_SetupLeaderboards(void);
void TimeAttackMenu_SetupLeaderboardsCarousel(EntityUICarousel *carousel);

#endif

#endif //! OBJ_TIMEATTACKMENU_H
