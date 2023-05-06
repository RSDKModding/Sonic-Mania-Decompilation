#ifndef OBJ_ENCOREINTRO_H
#define OBJ_ENCOREINTRO_H

#include "Game.h"

#if MANIA_USE_PLUS

// Object Class
struct ObjectEncoreIntro {
    RSDK_OBJECT
    int32 unused1;
    uint16 aniFrames;
    uint16 unused2;
    uint16 sfxHighFive;
    uint16 sfxMysticPoof;
    uint16 sfxKingCharge;
    uint16 sfxMysticHat;
    uint16 sfxMysticTransform;
    uint16 sfxPon;
    EntityFXRuby *fxRuby;
    EntityPhantomRuby *phantomRuby;
    EntityRubyPortal *rubyPortal;
    int32 unused3;
    bool32 awaitingSaveFinish;
};

// Entity Class
struct EntityEncoreIntro {
    RSDK_ENTITY
    Vector2 size;
    bool32 activated;
    int32 skipPart2;
    int32 unused1;
    int32 unused2;
    int32 unused3;
    int32 unused4;
    int32 unused5;
    Hitbox hitbox;

    bool32 seenActClear;
};

// Object Struct
extern ObjectEncoreIntro *EncoreIntro;

// Standard Entity Events
void EncoreIntro_Update(void);
void EncoreIntro_LateUpdate(void);
void EncoreIntro_StaticUpdate(void);
void EncoreIntro_Draw(void);
void EncoreIntro_Create(void *data);
void EncoreIntro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void EncoreIntro_EditorDraw(void);
void EncoreIntro_EditorLoad(void);
#endif
void EncoreIntro_Serialize(void);

// Extra Entity Functions
void EncoreIntro_SetupEntities(void);

void EncoreIntro_SetupCutscene(void);
void EncoreIntro_SetupCutscenePart2(void);

bool32 EncoreIntro_Cutscene_SetupAIZEncore(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_PlayerAppear(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_RubyAppear(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_PortalClose(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_Empty(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_BeginAIZEncore(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_AIZEncore(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_CapsuleFound(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_BuddySelect(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_ViewEncoreTutorial(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_MysticGetRuby(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_MysticStealRuby(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_MysticEscape(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_AIZEncoreTutorial(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_CameraPanToHBHPile(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_MysticPassRuby(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_KingActivate(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_RubyActivated(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_RubyWarp(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_LoadGHZ(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_AwaitSaveFinish(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_FadeOutAndReset(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_FadeInAndStart(EntityCutsceneSeq *host);
bool32 EncoreIntro_Cutscene_SkipAndFadeOut(EntityCutsceneSeq *host);

void EncoreIntro_SaveGameCB(bool32 success);

void EncoreIntro_PhantomRuby_OscillateFX(void);
void EncoreIntro_PhantomRuby_EscapeRight(void);
void EncoreIntro_PhantomRuby_Fall(void);
void EncoreIntro_PhantomRuby_CapsuleRiseUp(void);
void EncoreIntro_PhantomRuby_CapsuleFallDown(void);

void EncoreIntro_PlayerState_BuddySel(void);
void EncoreIntro_PlayerState_HandleAir(void);
void EncoreIntro_PlayerInput_BuddySel(void);

#endif

#endif //! OBJ_ENCOREINTRO_H
