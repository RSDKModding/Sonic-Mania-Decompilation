#ifndef OBJ_CPZ1INTRO_H
#define OBJ_CPZ1INTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 playerFrames;
    uint16 particleFrames;
    uint16 field_8;
    uint16 sfxChemDrop;
    uint16 sfxDNABurst;
    int32 playerAnimID;
    Entity *fxRuby;
    Entity *debris;
} ObjectCPZ1Intro;

// Entity Class
typedef struct {
    MANIA_CUTSCENE_BASE
} EntityCPZ1Intro;

// Object Struct
extern ObjectCPZ1Intro *CPZ1Intro;

// Standard Entity Events
void CPZ1Intro_Update(void);
void CPZ1Intro_LateUpdate(void);
void CPZ1Intro_StaticUpdate(void);
void CPZ1Intro_Draw(void);
void CPZ1Intro_Create(void* data);
void CPZ1Intro_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void CPZ1Intro_EditorDraw(void);
void CPZ1Intro_EditorLoad(void);
#endif
void CPZ1Intro_Serialize(void);

// Extra Entity Functions
void CPZ1Intro_Particle_CB(Entity *debris);
void CPZ1Intro_HandleRubyHover(Entity *player1, Entity *cutSeq, Entity *player2, int32 targetY);

bool32 CPZ1Intro_CheckSonicAnimFinish(void);
bool32 CPZ1Intro_CheckTailsAnimFinish(int32 timer);
bool32 CPZ1Intro_CheckKnuxAnimFinish(void);
#if RETRO_USE_PLUS
bool32 CPZ1Intro_CheckMightyAnimFinish(void);
bool32 CPZ1Intro_CheckRayAnimFinish(void);
#endif

bool32 CPZ1Intro_Cutscene_RubyWarp(void *h);
bool32 CPZ1Intro_Cutscene_PostWarpDrop(void *h);
bool32 CPZ1Intro_Cutscene_Waiting(void *h);
bool32 CPZ1Intro_Cutscene_ChemicalDrop(void *h);
bool32 CPZ1Intro_Cutscene_PlayerChemicalReact(void *h);
bool32 CPZ1Intro_Cutscene_ReadyStage(void *h);

#endif //!OBJ_CPZ1INTRO_H
