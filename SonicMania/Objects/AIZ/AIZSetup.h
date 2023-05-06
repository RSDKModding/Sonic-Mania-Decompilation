#ifndef OBJ_AIZSETUP_H
#define OBJ_AIZSETUP_H

#include "Game.h"

typedef enum {
    AIZ_BG_JUNGLE,
    AIZ_BG_SKY,
} BGSwitchIDsAIZ;

typedef enum {
    AIZ_DECOR_LEAF1,
    AIZ_DECOR_LEAF2,
    AIZ_DECOR_LEAF3,
    AIZ_DECOR_LEAF4,
} DecorTypesAIZ;

// Object Class
struct ObjectAIZSetup {
    RSDK_OBJECT
#if MANIA_USE_PLUS
    TABLE(int32 bellPlantAniFramePos[13], { 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, -1 });
    TABLE(int32 bellPlantAniDurations[12], { 6, 4, 3, 2, 3, 4, 6, 4, 3, 2, 3, 4 });
    TABLE(int32 curlPlant1AniFramePos[15], { 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, -1 });
    TABLE(int32 curlPlant1AniDurations[14], { 30, 4, 3, 2, 3, 4, 6, 12, 6, 4, 3, 2, 3, 4 });
    TABLE(int32 curlPlant2AniFramePos[15], { 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, -1 });
    TABLE(int32 curlPlant2AniDurations[14], { 12, 6, 4, 3, 2, 3, 4, 30, 4, 3, 2, 3, 4, 6 });
#endif
    bool32 hasSetupCutscene;
    bool32 playDrillSfx;
    bool32 playingDrillSfx;
    TileLayer *background1;
    TileLayer *background2;
    TileLayer *background3;
    TileLayer *background4;
    EntityAIZTornado *tornado;
    EntityAIZKingClaw *claw;
    EntityPlatform *platform;
    EntityPhantomRuby *phantomRuby;
    EntityFXRuby *fxRuby;
    EntityDecoration *decorations[3];
#if MANIA_USE_PLUS
    int32 bellPlantAniFrame;
    int32 bellPlantAniTimer;
    int32 curlPlant1AniFrame;
    int32 curlPlant1AniTimer;
    int32 curlPlant2AniFrame;
    int32 curlPlant2AniTimer;
#endif
    uint16 knuxFrames;
    uint16 sfxDrill;
    uint16 sfxBreak;
    uint16 sfxImpact;
    uint16 sfxHeliWoosh;
    uint16 aniTiles;
};

// Entity Class
struct EntityAIZSetup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectAIZSetup *AIZSetup;

// Standard Entity Events
void AIZSetup_Update(void);
void AIZSetup_LateUpdate(void);
void AIZSetup_StaticUpdate(void);
void AIZSetup_Draw(void);
void AIZSetup_Create(void *data);
void AIZSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void AIZSetup_EditorDraw(void);
void AIZSetup_EditorLoad(void);
#endif
void AIZSetup_Serialize(void);

// Extra Entity Functions
#if MANIA_USE_PLUS
void AIZSetup_BGSwitch_Jungle(void);
void AIZSetup_BGSwitch_Sky(void);
#endif

void AIZSetup_PlayerState_Static(void);
void AIZSetup_PlayerState_P2Enter(void);

void AIZSetup_HandleHeavyMovement(void);

void AIZSetup_SetupObjects(void);

void AIZSetup_GetCutsceneSetupPtr(void);
#if MANIA_USE_PLUS
void AIZSetup_Cutscene_SkipCB(void);
#endif

void AIZSetup_CutsceneST_Setup(void);
bool32 AIZSetup_CutsceneSonic_EnterAIZ(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneSonic_EnterAIZJungle(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneSonic_EnterHeavies(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneSonic_P2FlyIn(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneSonic_EnterClaw(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneSonic_WatchClaw(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneSonic_RubyGrabbed(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneSonic_RubyAppear(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneSonic_RubyFX(EntityCutsceneSeq *host);

void AIZSetup_CutsceneK_Setup(void);
bool32 AIZSetup_CutsceneKnux_Chillin(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneKnux_StartDrillin(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneKnux_Drillin(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneKnux_PrepareForTrouble(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneKnux_EnterThreat(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneKnux_HeaviesAppear(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneKnux_RubyImpact(EntityCutsceneSeq *host);
bool32 AIZSetup_CutsceneKnux_RubyFX(EntityCutsceneSeq *host);

bool32 AIZSetup_Cutscene_LoadGHZ(EntityCutsceneSeq *host);

#endif //! OBJ_AIZSETUP_H
