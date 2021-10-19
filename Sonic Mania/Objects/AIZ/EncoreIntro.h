#ifndef OBJ_ENCOREINTRO_H
#define OBJ_ENCOREINTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    uint16 aniFrames;
    uint16 field_A;
    uint16 sfxHighFive;
    uint16 sfxMysticPoof;
    uint16 sfxKingCharge;
    uint16 sfxMysticHat;
    uint16 sfxMysticTransform;
    uint16 sfxPon;
    Entity *fxRuby;
    Entity *phantomRuby;
    Entity *rubyPortal;
    int32 field_24;
    int32 field_28;
} ObjectEncoreIntro;

// Entity Class
typedef struct {
    MANIA_CUTSCENE_BASE
    int32 field_88;
} EntityEncoreIntro;

// Object Struct
extern ObjectEncoreIntro *EncoreIntro;

// Standard Entity Events
void EncoreIntro_Update(void);
void EncoreIntro_LateUpdate(void);
void EncoreIntro_StaticUpdate(void);
void EncoreIntro_Draw(void);
void EncoreIntro_Create(void* data);
void EncoreIntro_StageLoad(void);
void EncoreIntro_EditorDraw(void);
void EncoreIntro_EditorLoad(void);
void EncoreIntro_Serialize(void);

// Extra Entity Functions
void EncoreIntro_SetupEntities(void);

void EncoreIntro_SetupCutscene(void);
void EncoreIntro_SetupCutscenePart2(void);

bool32 EncoreIntro_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown2(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown3(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown4(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown5(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown6(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown7(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown8(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown9(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown10(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown11(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown12(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown13(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown14(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown15(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown16(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown17(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown18(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown19(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown20(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown21(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown22(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown23(EntityCutsceneSeq *host);
bool32 EncoreIntro_CutsceneState_Unknown24(EntityCutsceneSeq *host);

void EncoreIntro_SaveGameCB(int32 status);

void EncoreIntro_PhantomRuby_Unknown1(void);
void EncoreIntro_PhantomRuby_Unknown2(void);
void EncoreIntro_PhantomRuby_Unknown3(void);
void EncoreIntro_PhantomRuby_Unknown4(void);
void EncoreIntro_PhantomRuby_Unknown5(void);

void EncoreIntro_PlayerState_BuddySel(void);
void EncoreIntro_PlayerState_HandleAir(void);
void EncoreIntro_PlayerState_InputNone(void);

#endif //!OBJ_ENCOREINTRO_H
