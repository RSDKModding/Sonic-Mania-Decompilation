#ifndef OBJ_SSZ1INTRO_H
#define OBJ_SSZ1INTRO_H

#include "SonicMania.h"

// Object Class
struct ObjectSSZ1Intro {
    RSDK_OBJECT
    int32 field_4;
    int32 field_8;
    EntityFXRuby *fxRuby;
};

// Entity Class
struct EntitySSZ1Intro {
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
};

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

bool32 SSZ1Intro_Cutscene_FinishRubyWarp(EntityCutsceneSeq *host);
bool32 SSZ1Intro_Cutscene_HandeLanding(EntityCutsceneSeq *host);
bool32 SSZ1Intro_Cutscene_BeginAct1(EntityCutsceneSeq *host);

#endif //!OBJ_SSZ1INTRO_H
