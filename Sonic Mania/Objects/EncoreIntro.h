#ifndef OBJ_ENCOREINTRO_H
#define OBJ_ENCOREINTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    ushort aniFrames;
    ushort field_A;
    ushort sfxHighFive;
    ushort sfxMysticPoof;
    ushort sfxKingCharge;
    ushort sfxMysticHat;
    ushort sfxMysticTransform;
    ushort sfxPon;
    Entity *fxRuby;
    Entity *phantomRuby;
    Entity *rubyPortal;
    int field_24;
    int field_28;
} ObjectEncoreIntro;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    bool32 activated;
    bool32 startPart2;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    Hitbox hitbox;
    int field_88;
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

void EncoreIntro_SaveGameCB(int status);

void EncoreIntro_PhantomRuby_Unknown1(void);
void EncoreIntro_PhantomRuby_Unknown2(void);
void EncoreIntro_PhantomRuby_Unknown3(void);
void EncoreIntro_PhantomRuby_Unknown4(void);
void EncoreIntro_PhantomRuby_Unknown5(void);

void EncoreIntro_PlayerState_BuddySel(void);
void EncoreIntro_PlayerState_HandleAir(void);
void EncoreIntro_PlayerState_InputNone(void);

#endif //!OBJ_ENCOREINTRO_H
