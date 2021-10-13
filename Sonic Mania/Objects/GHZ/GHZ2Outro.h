#ifndef OBJ_GHZ2OUTRO_H
#define OBJ_GHZ2OUTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 field_4;
    uint16 field_6;
    uint16 sfxRocketJet;
    uint16 sfxShinobiJump;
    uint16 sfxHeliWoosh;
} ObjectGHZ2Outro;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int type;
    Vector2 size;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    Entity *eggman;
    Entity *DERobot;
    Entity *phantomRuby;
    Entity *fxRuby;
} EntityGHZ2Outro;

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
bool32 GHZ2Outro_CutsceneState1_Unknown1(EntityCutsceneSeq *host);
bool32 GHZ2Outro_CutsceneState1_Unknown2(EntityCutsceneSeq *host);

bool32 GHZ2Outro_CutsceneState2_Unknown1(EntityCutsceneSeq *host);
bool32 GHZ2Outro_CutsceneState2_Unknown2(EntityCutsceneSeq *host);
bool32 GHZ2Outro_CutsceneState2_Unknown3(EntityCutsceneSeq *host);
bool32 GHZ2Outro_CutsceneState2_Unknown4(EntityCutsceneSeq *host);
bool32 GHZ2Outro_CutsceneState2_Unknown5(EntityCutsceneSeq *host);
bool32 GHZ2Outro_CutsceneState2_Unknown6(EntityCutsceneSeq *host);
bool32 GHZ2Outro_LoadNextStage(EntityCutsceneSeq *host);

void GHZ2Outro_Cutscene_SkipCB(void);

#endif //!OBJ_GHZ2OUTRO_H
