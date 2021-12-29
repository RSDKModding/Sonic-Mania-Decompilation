#ifndef OBJ_GHZ2OUTRO_H
#define OBJ_GHZ2OUTRO_H

#include "SonicMania.h"

// Object Class
struct ObjectGHZ2Outro {
    RSDK_OBJECT
    uint16 field_4;
    uint16 field_6;
    uint16 sfxRocketJet;
    uint16 sfxShinobiJump;
    uint16 sfxHeliWoosh;
};

// Entity Class
struct EntityGHZ2Outro {
    MANIA_CUTSCENE_BASE
    Entity *eggman;
    Entity *DERobot;
    Entity *phantomRuby;
    Entity *fxRuby;
};

// Object Struct
extern ObjectGHZ2Outro *GHZ2Outro;

// Standard Entity Events
void GHZ2Outro_Update(void);
void GHZ2Outro_LateUpdate(void);
void GHZ2Outro_StaticUpdate(void);
void GHZ2Outro_Draw(void);
void GHZ2Outro_Create(void* data);
void GHZ2Outro_StageLoad(void);
void GHZ2Outro_EditorDraw(void);
void GHZ2Outro_EditorLoad(void);
void GHZ2Outro_Serialize(void);

// Extra Entity Functions
bool32 GHZ2Outro_Cutscene_FinishActClear(EntityCutsceneSeq *host);
bool32 GHZ2Outro_Cutscene_JumpIntoHole(EntityCutsceneSeq *host);

bool32 GHZ2Outro_Cutscene_HoleSceneFadeIn(EntityCutsceneSeq *host);
bool32 GHZ2Outro_Cutscene_SpyOnEggman(EntityCutsceneSeq *host);
bool32 GHZ2Outro_Cutscene_BreakupGroup(EntityCutsceneSeq *host);
bool32 GHZ2Outro_Cutscene_RubyHover(EntityCutsceneSeq *host);
bool32 GHZ2Outro_Cutscene_StartRubyWarp(EntityCutsceneSeq *host);
bool32 GHZ2Outro_Cutscene_HandleRubyWarp(EntityCutsceneSeq *host);
bool32 GHZ2Outro_Cutscene_LoadCPZ1(EntityCutsceneSeq *host);

#if RETRO_USE_PLUS
void GHZ2Outro_Cutscene_SkipCB(void);
#endif

#endif //!OBJ_GHZ2OUTRO_H
