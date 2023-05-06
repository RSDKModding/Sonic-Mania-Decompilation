#ifndef OBJ_CPZ1INTRO_H
#define OBJ_CPZ1INTRO_H

#include "Game.h"

// Object Class
struct ObjectCPZ1Intro {
    RSDK_OBJECT
    uint16 playerFrames;
    uint16 particleFrames;
    uint16 unused;
    uint16 sfxChemDrop;
    uint16 sfxDNABurst;
    int32 playerAnimID;
    EntityFXRuby *fxRuby;
    EntityDebris *debris;
};

// Entity Class
struct EntityCPZ1Intro {
    MANIA_CUTSCENE_BASE
};

// Object Struct
extern ObjectCPZ1Intro *CPZ1Intro;

// Standard Entity Events
void CPZ1Intro_Update(void);
void CPZ1Intro_LateUpdate(void);
void CPZ1Intro_StaticUpdate(void);
void CPZ1Intro_Draw(void);
void CPZ1Intro_Create(void *data);
void CPZ1Intro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CPZ1Intro_EditorDraw(void);
void CPZ1Intro_EditorLoad(void);
#endif
void CPZ1Intro_Serialize(void);

// Extra Entity Functions
void CPZ1Intro_Particle_ChemDrop(EntityDebris *debris);
void CPZ1Intro_HandleRubyHover(EntityCutsceneSeq *cutsceneSequence, EntityPlayer *player1, EntityPlayer *player2, int32 targetY);

bool32 CPZ1Intro_CheckSonicAnimFinish(void);
bool32 CPZ1Intro_CheckTailsAnimFinish(int32 timer);
bool32 CPZ1Intro_CheckKnuxAnimFinish(void);
#if MANIA_USE_PLUS
bool32 CPZ1Intro_CheckMightyAnimFinish(void);
bool32 CPZ1Intro_CheckRayAnimFinish(void);
#endif

bool32 CPZ1Intro_Cutscene_RubyWarp(EntityCutsceneSeq *host);
bool32 CPZ1Intro_Cutscene_PostWarpDrop(EntityCutsceneSeq *host);
bool32 CPZ1Intro_Cutscene_Waiting(EntityCutsceneSeq *host);
bool32 CPZ1Intro_Cutscene_ChemicalDrop(EntityCutsceneSeq *host);
bool32 CPZ1Intro_Cutscene_PlayerChemicalReact(EntityCutsceneSeq *host);
bool32 CPZ1Intro_Cutscene_ReadyStage(EntityCutsceneSeq *host);

#endif //! OBJ_CPZ1INTRO_H
