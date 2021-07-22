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
    int field_58;
    bool32 flag;
    EntityPhantomRuby *ruby;
    EntityFXRuby *fxRuby;
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
void SSZ3Cutscene_Unknown(void);

bool32 SSZ3Cutscene_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_CutsceneState_Unknown2(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_CutsceneState_Unknown4(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_CutsceneState_Unknown5(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_CutsceneState_Unknown6(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_CutsceneState_Unknown7(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_CutsceneState_Unknown8(EntityCutsceneSeq *host);
bool32 SSZ3Cutscene_CutsceneState_Unknown9(EntityCutsceneSeq *host);

#endif //!OBJ_SSZ3CUTSCENE_H
