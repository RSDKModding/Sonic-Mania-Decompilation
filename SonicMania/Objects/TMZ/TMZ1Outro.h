#ifndef OBJ_TMZ1OUTRO_H
#define OBJ_TMZ1OUTRO_H

#include "Game.h"

// Object Class
struct ObjectTMZ1Outro {
    RSDK_OBJECT
    int32 unused;
};

// Entity Class
struct EntityTMZ1Outro {
    MANIA_CUTSCENE_BASE
};

// Object Struct
extern ObjectTMZ1Outro *TMZ1Outro;

// Standard Entity Events
void TMZ1Outro_Update(void);
void TMZ1Outro_LateUpdate(void);
void TMZ1Outro_StaticUpdate(void);
void TMZ1Outro_Draw(void);
void TMZ1Outro_Create(void *data);
void TMZ1Outro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TMZ1Outro_EditorDraw(void);
void TMZ1Outro_EditorLoad(void);
#endif
void TMZ1Outro_Serialize(void);

// Extra Entity Functions
bool32 TMZ1Outro_CutsceneAct1_SetupPlayers(EntityCutsceneSeq *host);
bool32 TMZ1Outro_CutsceneAct1_ElevatorRide(EntityCutsceneSeq *host);
bool32 TMZ1Outro_CutsceneAct1_HeadForEntrance(EntityCutsceneSeq *host);
bool32 TMZ1Outro_CutsceneAct1_EnterMonarch(EntityCutsceneSeq *host);

bool32 TMZ1Outro_CutsceneAct2_BeginAct2(EntityCutsceneSeq *host);

#endif //! OBJ_TMZ1OUTRO_H
