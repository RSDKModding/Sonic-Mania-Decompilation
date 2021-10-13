#ifndef OBJ_SSZ3CUTSCENE_H
#define OBJ_SSZ3CUTSCENE_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSSZ3Cutscene;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
#if RETRO_USE_PLUS
    bool32 flag;
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
void SSZ3Cutscene_Create(void* data);
void SSZ3Cutscene_StageLoad(void);
void SSZ3Cutscene_EditorDraw(void);
void SSZ3Cutscene_EditorLoad(void);
void SSZ3Cutscene_Serialize(void);

// Extra Entity Functions
#if RETRO_USE_PLUS
void SSZ3Cutscene_HandleRubyFX(void);
#endif

bool32 SSZ3Cutscene_IntroState_Unknown1(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_IntroState_Unknown2(EntityCutsceneSeq *host);
#if RETRO_USE_PLUS
bool32 SSZ3Cutscene_OutroState_Unknown1(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_OutroState_Unknown2(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_OutroState_Unknown3(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_OutroState_Unknown4(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_OutroState_Unknown5(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_OutroState_LoadNextScene(EntityCutsceneSeq *host);
#endif

#endif //!OBJ_SSZ3CUTSCENE_H
