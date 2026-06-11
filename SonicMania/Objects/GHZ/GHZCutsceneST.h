#ifndef OBJ_GHZCUTSCENEST_H
#define OBJ_GHZCUTSCENEST_H

#include "Game.h"
#include "AIZ/AIZKingClaw.h"
#include "Common/Platform.h"
#include "Cutscene/CutsceneHBH.h"
#include "Cutscene/CutsceneSeq.h"
#include "Cutscene/FXRuby.h"
#include "ERZ/PhantomRuby.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 unused1;
    EntityFXRuby *fxRuby;
    EntityAIZKingClaw *claw;
    EntityPlatform *platform;
    EntityPhantomRuby *phantomRuby;
    EntityCutsceneHBH *cutsceneHBH[5];
} ObjectGHZCutsceneST;

// Entity Class
typedef struct {
    RSDK_ENTITY
    // Cutscene Base (I think)
    Vector2 size;
    bool32 activated;
    bool32 setupKnuxCutscene;
    int32 timer;
    int32 unused1;
    int32 unused2;
    int32 unused3;
    int32 unused4;
    Hitbox hitboxUnused;

    // Unique Variables
    Hitbox hitbox;
} EntityGHZCutsceneST;

// Object Struct
extern ObjectGHZCutsceneST *GHZCutsceneST;

// Standard Entity Events
void GHZCutsceneST_Update(void);
void GHZCutsceneST_LateUpdate(void);
void GHZCutsceneST_StaticUpdate(void);
void GHZCutsceneST_Draw(void);
void GHZCutsceneST_Create(void *data);
void GHZCutsceneST_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void GHZCutsceneST_EditorDraw(void);
void GHZCutsceneST_EditorLoad(void);
#endif
void GHZCutsceneST_Serialize(void);

// Extra Entity Functions
void GHZCutsceneST_SetupObjects(void);
void GHZCutsceneST_SetupKnuxCutscene(void);
#if MANIA_USE_PLUS
void GHZCutsceneST_Cutscene_SkipCB(void);
#endif

bool32 GHZCutsceneST_Cutscene_FadeIn(EntityCutsceneSeq *host);
bool32 GHZCutsceneST_Cutscene_FinishRubyWarp(EntityCutsceneSeq *host);
bool32 GHZCutsceneST_Cutscene_ExitHBH(EntityCutsceneSeq *host);
bool32 GHZCutsceneST_Cutscene_SetupGHZ1(EntityCutsceneSeq *host);

#endif //! OBJ_GHZCUTSCENEST_H
