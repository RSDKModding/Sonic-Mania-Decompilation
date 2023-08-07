#ifndef OBJ_SSZ1INTRO_H
#define OBJ_SSZ1INTRO_H

#include "Game.h"

// Object Class
struct ObjectSSZ1Intro {
    RSDK_OBJECT
    int32 unused1;
    int32 unused2;
    EntityFXRuby *fxRuby;
};

// Entity Class
struct EntitySSZ1Intro {
    MANIA_CUTSCENE_BASE
};

// Object Struct
extern ObjectSSZ1Intro *SSZ1Intro;

// Standard Entity Events
void SSZ1Intro_Update(void);
void SSZ1Intro_LateUpdate(void);
void SSZ1Intro_StaticUpdate(void);
void SSZ1Intro_Draw(void);
void SSZ1Intro_Create(void *data);
void SSZ1Intro_StageLoad(void);
void SSZ1Intro_EditorDraw(void);
void SSZ1Intro_EditorLoad(void);
void SSZ1Intro_Serialize(void);

// Extra Entity Functions
void SSZ1Intro_SetupCutscene(void);
void SSZ1Intro_HandleRubyHover(EntityCutsceneSeq *host, EntityPlayer *player1, EntityPlayer *player2, int32 offset);

bool32 SSZ1Intro_Cutscene_FinishRubyWarp(EntityCutsceneSeq *host);
bool32 SSZ1Intro_Cutscene_HandleLanding(EntityCutsceneSeq *host);
bool32 SSZ1Intro_Cutscene_BeginAct1(EntityCutsceneSeq *host);

#endif //! OBJ_SSZ1INTRO_H
