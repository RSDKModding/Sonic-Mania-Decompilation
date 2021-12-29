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
    int32 timer;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    Hitbox hitbox;
    int32 field_84;
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
bool32 SPZ1Intro_Cutscene_SetupAct(EntityCutsceneSeq *host);
bool32 SPZ1Intro_Cutscene_ExitPipe(EntityCutsceneSeq *host);
bool32 SPZ1Intro_Cutscene_BeginAct1(EntityCutsceneSeq *host);

#endif //!OBJ_SPZ1INTRO_H
