#ifndef OBJ_SPZ1INTRO_H
#define OBJ_SPZ1INTRO_H

#include "Game.h"

// Object Class
struct ObjectSPZ1Intro {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxGasPop;
    uint16 sfxPon;
    EntityDebris *sewerLid;
};

// Entity Class
struct EntitySPZ1Intro {
    MANIA_CUTSCENE_BASE

    int32 unused;
};

// Object Struct
extern ObjectSPZ1Intro *SPZ1Intro;

// Standard Entity Events
void SPZ1Intro_Update(void);
void SPZ1Intro_LateUpdate(void);
void SPZ1Intro_StaticUpdate(void);
void SPZ1Intro_Draw(void);
void SPZ1Intro_Create(void *data);
void SPZ1Intro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SPZ1Intro_EditorDraw(void);
void SPZ1Intro_EditorLoad(void);
#endif
void SPZ1Intro_Serialize(void);

// Extra Entity Functions
bool32 SPZ1Intro_Cutscene_SetupAct(EntityCutsceneSeq *host);
bool32 SPZ1Intro_Cutscene_ExitPipe(EntityCutsceneSeq *host);
bool32 SPZ1Intro_Cutscene_BeginAct1(EntityCutsceneSeq *host);

#endif //! OBJ_SPZ1INTRO_H
