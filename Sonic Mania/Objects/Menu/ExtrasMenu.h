#ifndef OBJ_EXTRASMENU_H
#define OBJ_EXTRASMENU_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    Entity *extrasControl;
    int32 cheatCode[8];
} ObjectExtrasMenu;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityExtrasMenu;

// Object Struct
extern ObjectExtrasMenu *ExtrasMenu;

// Standard Entity Events
void ExtrasMenu_Update(void);
void ExtrasMenu_LateUpdate(void);
void ExtrasMenu_StaticUpdate(void);
void ExtrasMenu_Draw(void);
void ExtrasMenu_Create(void* data);
void ExtrasMenu_StageLoad(void);
void ExtrasMenu_EditorDraw(void);
void ExtrasMenu_EditorLoad(void);
void ExtrasMenu_Serialize(void);

// Extra Entity Functions
void ExtrasMenu_Initialize(void);
void ExtrasMenu_Unknown2(void);
void ExtrasMenu_Unknown3(void);

void ExtrasMenu_ProcessMedallionCheat(void);
bool32 ExtrasMenu_CheckMedallionCheat(void);
void ExtrasMenu_ProcessInputs(void);

void ExtrasMenu_Start_Puyo_vsAI(void);
void ExtrasMenu_Callback_Puyo_vsAI(void);
void ExtrasMenu_Start_Puyo_vs2P(void);
void ExtrasMenu_Callback_Puyo_vs2P(void);
void ExtrasMenu_Start_Credits(void);
void ExtrasMenu_Callback_Credits(void);
void ExtrasMenu_Start_DAGarden(void);
void ExtrasMenu_Callback_DAGarden(void);
void ExtrasMenu_Start_BSS_3K(void);
void ExtrasMenu_Callback_BSS_3K(void);
void ExtrasMenu_Start_BSS_Mania(void);
void ExtrasMenu_Callback_BSS_Mania(void);
#endif

#endif //!OBJ_EXTRASMENU_H
