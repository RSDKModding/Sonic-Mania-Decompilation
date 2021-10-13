#ifndef OBJ_SPZ1INTRO_H
#define OBJ_SPZ1INTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxGasPop;
    uint16 sfxPon;
    EntityDebris *debris;
} ObjectSPZ1Intro;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    int timer;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    Hitbox hitbox;
    int field_84;
} EntitySPZ1Intro;

// Object Struct
extern ObjectSPZ1Intro *SPZ1Intro;

// Standard Entity Events
void SPZ1Intro_Update(void);
void SPZ1Intro_LateUpdate(void);
void SPZ1Intro_StaticUpdate(void);
void SPZ1Intro_Draw(void);
void SPZ1Intro_Create(void* data);
void SPZ1Intro_StageLoad(void);
void SPZ1Intro_EditorDraw(void);
void SPZ1Intro_EditorLoad(void);
void SPZ1Intro_Serialize(void);

// Extra Entity Functions
bool32 SPZ1Intro_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 SPZ1Intro_CutsceneState_Unknown2(EntityCutsceneSeq *host);
bool32 SPZ1Intro_CutsceneState_Unknown3(EntityCutsceneSeq *host);

#endif //!OBJ_SPZ1INTRO_H
