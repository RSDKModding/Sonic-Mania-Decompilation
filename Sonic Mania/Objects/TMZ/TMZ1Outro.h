#ifndef OBJ_TMZ1OUTRO_H
#define OBJ_TMZ1OUTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
} ObjectTMZ1Outro;

// Entity Class
typedef struct {
    MANIA_CUTSCENE_BASE
} EntityTMZ1Outro;

// Object Struct
extern ObjectTMZ1Outro *TMZ1Outro;

// Standard Entity Events
void TMZ1Outro_Update(void);
void TMZ1Outro_LateUpdate(void);
void TMZ1Outro_StaticUpdate(void);
void TMZ1Outro_Draw(void);
void TMZ1Outro_Create(void* data);
void TMZ1Outro_StageLoad(void);
void TMZ1Outro_EditorDraw(void);
void TMZ1Outro_EditorLoad(void);
void TMZ1Outro_Serialize(void);

// Extra Entity Functions
bool32 TMZ1Outro_CutsceneAct1_SetupPlayers(EntityCutsceneSeq *host);
bool32 TMZ1Outro_CutsceneAct1_ElevatorRide(EntityCutsceneSeq *host);
bool32 TMZ1Outro_CutsceneAct1_HeadForEntrance(EntityCutsceneSeq *host);
bool32 TMZ1Outro_CutsceneAct1_EnterMonarch(EntityCutsceneSeq *host);

bool32 TMZ1Outro_CutsceneAct2_BeginAct2(EntityCutsceneSeq *host);

#endif //!OBJ_TMZ1OUTRO_H
