#ifndef OBJ_COMPETITIONMENU_H
#define OBJ_COMPETITIONMENU_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    Entity *competitionControl;
    Entity *competitionControl_Legacy;
    Entity *compRulesControl;
    Entity *compZoneControl;
    int32 field_14;
    Entity *prompt1;
    Entity *prompt2;
    Entity *compRoundControl;
    Entity *label1;
    Entity *compTotalControl;
    Entity *label2;
    int32 timer;
} ObjectCompetitionMenu;

// Entity Class
typedef struct {
    RSDK_ENTITY
} EntityCompetitionMenu;

// Object Struct
extern ObjectCompetitionMenu *CompetitionMenu;

// Standard Entity Events
void CompetitionMenu_Update(void);
void CompetitionMenu_LateUpdate(void);
void CompetitionMenu_StaticUpdate(void);
void CompetitionMenu_Draw(void);
void CompetitionMenu_Create(void* data);
void CompetitionMenu_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void CompetitionMenu_EditorDraw(void);
void CompetitionMenu_EditorLoad(void);
#endif
void CompetitionMenu_Serialize(void);

// Extra Entity Functions
void CompetitionMenu_Initialize(void);
void CompetitionMenu_Unknown2(void);
void CompetitionMenu_Unknown3(void);
int32 CompetitionMenu_Unknown4(void);
void CompetitionMenu_Unknown5(void);
int32 CompetitionMenu_GetReadyPlayerCount(void);
int32 CompetitionMenu_GetFlaggedReadyPlayerCount(void);
void CompetitionMenu_Unknown8(void);
void CompetitionMenu_Unknown9(int32 playerCount);
void CompetitionMenu_SetupSplitScreen(int32 mode);
void CompetitionMenu_Unknown11(void *control);
void CompetitionMenu_GotoCompRules(void);
void CompetitionMenu_GotoCompZones(void);
void CompetitionMenu_VS_ProcessInputCB(void);
void CompetitionMenu_VS_MenuSetupCB(void);
void CompetitionMenu_Rules_MenuSetupCB(void);
void CompetitionMenu_StartMatch(void);
void CompetitionMenu_ZoneButtonActionCB(void);
void CompetitionMenu_RulesButtonActionCB(void);
void CompetitionMenu_GotoCompTotal(void);
void CompetitionMenu_Round_ProcessInputCB(void);
void CompetitionMenu_Round_MenuSetupCB(void);
void CompetitionMenu_GotoCompetition(void);
void CompetitionMenu_Results_ProcessInputCB(void);
void CompetitionMenu_Results_MenuSetupCB(void);
void CompetitionMenu_Results_MenuUpdateCB(void);
void CompetitionMenu_ExitComp_TransitionCB(void);
void CompetitionMenu_ExitComp_YesCB(void);
bool32 CompetitionMenu_Unknown29(void);
bool32 CompetitionMenu_Unknown30(void);
void CompetitionMenu_GotoPuyoVS(void);
#endif

#endif //!OBJ_COMPETITIONMENU_H
