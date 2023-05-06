#ifndef OBJ_EXTRASMENU_H
#define OBJ_EXTRASMENU_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectExtrasMenu {
    RSDK_OBJECT
    EntityUIControl *extrasControl;
    int32 cheatCode[8];
};

// Entity Class
struct EntityExtrasMenu {
    RSDK_ENTITY
};

// Object Struct
extern ObjectExtrasMenu *ExtrasMenu;

// Standard Entity Events
void ExtrasMenu_Update(void);
void ExtrasMenu_LateUpdate(void);
void ExtrasMenu_StaticUpdate(void);
void ExtrasMenu_Draw(void);
void ExtrasMenu_Create(void *data);
void ExtrasMenu_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ExtrasMenu_EditorDraw(void);
void ExtrasMenu_EditorLoad(void);
#endif
void ExtrasMenu_Serialize(void);

// Extra Entity Functions
void ExtrasMenu_Initialize(void);
void ExtrasMenu_HandleUnlocks(void);
void ExtrasMenu_SetupActions(void);

void ExtrasMenu_ProcessMedallionCheat(void);
bool32 ExtrasMenu_CheckMedallionCheat(void);
void ExtrasMenu_ProcessInputs(void);

void ExtrasMenu_Start_Puyo_vsAI(void);
void ExtrasMenu_Puyo_vsAI_ActionCB(void);
void ExtrasMenu_Start_Puyo_vs2P(void);
void ExtrasMenu_Puyo_vs2P_ActionCB(void);
void ExtrasMenu_Start_Credits(void);
void ExtrasMenu_CreditsButton_ActionCB(void);
void ExtrasMenu_Start_DAGarden(void);
void ExtrasMenu_DAGarden_ActionCB(void);
void ExtrasMenu_Start_BSS_3K(void);
void ExtrasMenu_BSS_S3_ActionCB(void);
void ExtrasMenu_Start_BSS_Mania(void);
void ExtrasMenu_BSS_Mania_ActionCB(void);
#endif

#endif //! OBJ_EXTRASMENU_H
