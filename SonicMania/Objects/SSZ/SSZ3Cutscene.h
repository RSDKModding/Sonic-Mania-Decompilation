#ifndef OBJ_SSZ3CUTSCENE_H
#define OBJ_SSZ3CUTSCENE_H

#include "Game.h"
#include "Cutscene/CutsceneSeq.h"
#if MANIA_USE_PLUS
#include "Cutscene/FXRuby.h"
#include "ERZ/PhantomRuby.h"
#endif

// Object Class
typedef struct {
    RSDK_OBJECT
} ObjectSSZ3Cutscene;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 unused;
#if MANIA_USE_PLUS
    bool32 playTransformCutscene;
    EntityPhantomRuby *ruby;
    EntityFXRuby *fxRuby;
#endif
} EntitySSZ3Cutscene;

// Object Struct
extern ObjectSSZ3Cutscene *SSZ3Cutscene;

// Standard Entity Events
void SSZ3Cutscene_Update(void);
void SSZ3Cutscene_LateUpdate(void);
void SSZ3Cutscene_StaticUpdate(void);
void SSZ3Cutscene_Draw(void);
void SSZ3Cutscene_Create(void *data);
void SSZ3Cutscene_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SSZ3Cutscene_EditorDraw(void);
void SSZ3Cutscene_EditorLoad(void);
#endif
void SSZ3Cutscene_Serialize(void);

// Extra Entity Functions
#if MANIA_USE_PLUS
void SSZ3Cutscene_HandleRubyFX(void);
#endif

bool32 SSZ3Cutscene_CutsceneIntro_EnterStageLeft(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_CutsceneIntro_PlayerRunLeft(EntityCutsceneSeq *host);
#if MANIA_USE_PLUS
bool32 SSZ3Cutscene_CutsceneOutro_SetupOutro(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_CutsceneOutro_FollowRuby(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_CutsceneOutro_EnterRuby(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_CutsceneOutro_RubyActivate(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_CutsceneOutro_RubyWarp(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_CutsceneOutro_LoadHCZ1(EntityCutsceneSeq *host);
#endif

#endif //! OBJ_SSZ3CUTSCENE_H
