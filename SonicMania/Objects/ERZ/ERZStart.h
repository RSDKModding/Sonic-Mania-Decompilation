#ifndef OBJ_ERZSTART_H
#define OBJ_ERZSTART_H

#include "Game.h"

// Object Class
struct ObjectERZStart {
    RSDK_OBJECT
    int32 timer;
    int32 unused1;
    int32 superDashCooldown;
    int32 unused2;
    uint16 sfxClack;
    uint16 sfxFlyIn;
    uint16 sfxHullClose;
    uint16 sfxRumble;
    EntityFXRuby *fxRuby;
    EntityPhantomRuby *ruby;
    EntityChaosEmerald *emeralds[7];
    EntityPhantomKing *king;
    EntityKleptoMobile *eggman;
};

// Entity Class
struct EntityERZStart {
    MANIA_CUTSCENE_BASE
    int32 timer;
};

// Object Struct
extern ObjectERZStart *ERZStart;

// Standard Entity Events
void ERZStart_Update(void);
void ERZStart_LateUpdate(void);
void ERZStart_StaticUpdate(void);
void ERZStart_Draw(void);
void ERZStart_Create(void *data);
void ERZStart_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ERZStart_EditorDraw(void);
void ERZStart_EditorLoad(void);
#endif
void ERZStart_Serialize(void);

// Extra Entity Functions
void ERZStart_SetupObjects(void);
void ERZStart_HandlePlayerHover(EntityCutsceneSeq *seq, EntityPlayer *player, int32 posY);

bool32 ERZStart_Cutscene_FadeIn(EntityCutsceneSeq *host);
bool32 ERZStart_Cutscene_ShrinkRubyWarpFX(EntityCutsceneSeq *host);
bool32 ERZStart_Cutscene_EnterKing(EntityCutsceneSeq *host);
bool32 ERZStart_Cutscene_KingMovingRuby(EntityCutsceneSeq *host);
bool32 ERZStart_Cutscene_KingAttatchHornRuby(EntityCutsceneSeq *host);
bool32 ERZStart_Cutscene_SetupEggmanReveal(EntityCutsceneSeq *host);
bool32 ERZStart_Cutscene_EnterEggman(EntityCutsceneSeq *host);
bool32 ERZStart_Cutscene_EggmanKingWrestling(EntityCutsceneSeq *host);
bool32 ERZStart_Cutscene_PostWrestleFadeIn(EntityCutsceneSeq *host);
bool32 ERZStart_Cutscene_ReturnCamToSonic(EntityCutsceneSeq *host);
bool32 ERZStart_Cutscene_PreparePlayerTransform(EntityCutsceneSeq *host);
bool32 ERZStart_Cutscene_PlayerTransform(EntityCutsceneSeq *host);
bool32 ERZStart_Cutscene_StartFight(EntityCutsceneSeq *host);
bool32 ERZStart_Cutscene_Fight(EntityCutsceneSeq *host);

void ERZStart_RubyHover(void);
void ERZStart_RubyMove(void);

void ERZStart_Player_HandleSuperDash(EntityPlayer *player);
void ERZStart_State_PlayerSuperFly(void);
void ERZStart_Player_StartSuperFly(void);
void ERZStart_State_PlayerRebound(void);

#endif //! OBJ_ERZSTART_H
