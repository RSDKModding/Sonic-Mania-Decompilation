#ifndef OBJ_SSZ1INTRO_H
#define OBJ_SSZ1INTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    int32 field_8;
    EntityFXRuby *fxRuby;
} ObjectSSZ1Intro;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    bool32 activated;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    Hitbox hitbox;
} EntitySSZ1Intro;

// Object Struct
extern ObjectSSZ1Intro *SSZ1Intro;

// Standard Entity Events
void SSZ1Intro_Update(void);
void SSZ1Intro_LateUpdate(void);
void SSZ1Intro_StaticUpdate(void);
void SSZ1Intro_Draw(void);
void SSZ1Intro_Create(void* data);
void SSZ1Intro_StageLoad(void);
void SSZ1Intro_EditorDraw(void);
void SSZ1Intro_EditorLoad(void);
void SSZ1Intro_Serialize(void);

// Extra Entity Functions
void SSZ1Intro_Unknown1(EntityPlayer *player1, EntityCutsceneSeq *host, EntityPlayer *player2, int32 offset);

bool32 SSZ1Intro_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 SSZ1Intro_CutsceneState_Unknown2(EntityCutsceneSeq *host);
bool32 SSZ1Intro_CutsceneState_Unknown3(EntityCutsceneSeq *host);

#endif //!OBJ_SSZ1INTRO_H
