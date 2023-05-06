#ifndef OBJ_COMPETITIONMENU_H
#define OBJ_COMPETITIONMENU_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectCompetitionMenu {
    RSDK_OBJECT
    EntityUIControl *competitionControl;
    EntityUIControl *competitionControl_Legacy;
    EntityUIControl *compRulesControl;
    EntityUIControl *compZoneControl;
    int32 unused;
    EntityUIButtonPrompt *startCompPrompt;
    EntityUIButtonPrompt *startCompPrompt_Legacy;
    EntityUIControl *compRoundControl;
    EntityUIInfoLabel *resultsLabel_Round;
    EntityUIControl *compTotalControl;
    EntityUIInfoLabel *resultsLabel_Total;
    int32 timer;
};

// Entity Class
struct EntityCompetitionMenu {
    RSDK_ENTITY
};

// Object Struct
extern ObjectCompetitionMenu *CompetitionMenu;

// Standard Entity Events
void CompetitionMenu_Update(void);
void CompetitionMenu_LateUpdate(void);
void CompetitionMenu_StaticUpdate(void);
void CompetitionMenu_Draw(void);
void CompetitionMenu_Create(void *data);
void CompetitionMenu_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CompetitionMenu_EditorDraw(void);
void CompetitionMenu_EditorLoad(void);
#endif
void CompetitionMenu_Serialize(void);

// Extra Entity Functions
void CompetitionMenu_Initialize(void);
void CompetitionMenu_SetupActions(void);
void CompetitionMenu_HandleMenuReturn(void);
int32 CompetitionMenu_HandleUnlocks(void);
void CompetitionMenu_HandleStartCompPrompt(void);
int32 CompetitionMenu_GetReadyPlayerCount(void);
int32 CompetitionMenu_GetTotalPlayerCount(void);
void CompetitionMenu_ResetControllerAssignments(void);
void CompetitionMenu_SetupSplitScreenChoices(int32 playerCount);
void CompetitionMenu_SetupSplitScreen(int32 mode);
void CompetitionMenu_SetupResultsUI(EntityUIControl *roundControl);
void CompetitionMenu_GotoCompRules(void);
void CompetitionMenu_GotoCompZones(void);
void CompetitionMenu_VS_ProcessInputCB(void);
void CompetitionMenu_VS_MenuSetupCB(void);
void CompetitionMenu_Rules_MenuSetupCB(void);
void CompetitionMenu_StartMatch(void);
void CompetitionMenu_ZoneButtonActionCB(void);
void CompetitionMenu_RulesButton_ActionCB(void);
void CompetitionMenu_GotoCompTotal(void);
void CompetitionMenu_Round_ProcessInputCB(void);
void CompetitionMenu_Round_MenuSetupCB(void);
void CompetitionMenu_GotoCompetition(void);
void CompetitionMenu_Results_ProcessInputCB(void);
void CompetitionMenu_Results_MenuSetupCB(void);
void CompetitionMenu_Results_MenuUpdateCB(void);
void CompetitionMenu_ExitComp_TransitionCB(void);
void CompetitionMenu_ExitComp_YesCB(void);
bool32 CompetitionMenu_CompRules_BackPressCB(void);
bool32 CompetitionMenu_CompZones_BackPressCB(void);
void CompetitionMenu_GotoPuyoVS(void);
#endif

#endif //! OBJ_COMPETITIONMENU_H
