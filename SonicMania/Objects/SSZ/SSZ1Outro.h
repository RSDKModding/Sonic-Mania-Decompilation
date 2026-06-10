#ifndef OBJ_SSZ1OUTRO_H
#define OBJ_SSZ1OUTRO_H

#include "Game.h"
#include "HotaruMKII.h"
#include "RTeleporter.h"
#include "Cutscene/CutsceneSeq.h"
#include "Cutscene/FXFade.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 unused;
    EntityFXFade *fxFade;
    EntityRTeleporter *teleporter;
} ObjectSSZ1Outro;

// Entity Class
typedef struct {
    MANIA_CUTSCENE_BASE
} EntitySSZ1Outro;

// Object Struct
extern ObjectSSZ1Outro *SSZ1Outro;

// Standard Entity Events
void SSZ1Outro_Update(void);
void SSZ1Outro_LateUpdate(void);
void SSZ1Outro_StaticUpdate(void);
void SSZ1Outro_Draw(void);
void SSZ1Outro_Create(void *data);
void SSZ1Outro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SSZ1Outro_EditorDraw(void);
void SSZ1Outro_EditorLoad(void);
#endif
void SSZ1Outro_Serialize(void);

// Extra Entity Functions
bool32 SSZ1Outro_Cutscene_TimeWarpRunway(EntityCutsceneSeq *host);
bool32 SSZ1Outro_Cutscene_TimeWarp(EntityCutsceneSeq *host);

void SSZ1Outro_DestroyHotaru(EntityHotaruMKII *hotaru);
void SSZ1Outro_Trigger_DestroyHotarus(void);

#endif //! OBJ_SSZ1OUTRO_H
