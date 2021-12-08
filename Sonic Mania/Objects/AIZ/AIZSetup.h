#ifndef OBJ_AIZSETUP_H
#define OBJ_AIZSETUP_H

#include "SonicMania.h"

typedef enum {
    AIZ_BG_JUNGLE,
    AIZ_BG_SKY,
} BGSwitchIDsAIZ;

// Object Class
typedef struct {
    RSDK_OBJECT
#if RETRO_USE_PLUS
    TABLE(int32 aniTiles1[13], { 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, -1 });
    TABLE(int32 aniTiles2[12], { 6, 4, 3, 2, 3, 4, 6, 4, 3, 2, 3, 4 });
    TABLE(int32 aniTiles3[15], { 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, -1 });
    TABLE(int32 aniTiles4[14], { 30, 4, 3, 2, 3, 4, 6, 12, 6, 4, 3, 2, 3, 4 });
    TABLE(int32 aniTiles5[15], { 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, -1 });
    TABLE(int32 aniTiles6[14], { 12, 6, 4, 3, 2, 3, 4, 30, 4, 3, 2, 3, 4, 6 });
#endif
    bool32 cutsceneInit;
    bool32 playDrillSfxFlag;
    bool32 playingDrillSFX;
    TileLayer *bg1Info;
    TileLayer *bg2Info;
    TileLayer *bg3Info;
    TileLayer *bg4Info;
    Entity *tornado;
    Entity *claw;
    Entity *platform;
    Entity *phantomRuby;
    Entity *fxRuby;
    Entity *decorations[3];
#if RETRO_USE_PLUS
    int32 aniTileFrameA;
    int32 aniTilesDelay1;
    int32 aniTileFrameB;
    int32 aniTilesDelay2;
    int32 aniTileFrameC;
    int32 aniTilesDelay3;
#endif  
    uint16 knuxSpriteIndex;
    uint16 sfxDrill;
    uint16 sfxBreak;
    uint16 sfxImpact;
    uint16 sfxHeliWoosh;
    uint16 aniTiles;
} ObjectAIZSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityAIZSetup;

// Object Struct
extern ObjectAIZSetup *AIZSetup;

// Standard Entity Events
void AIZSetup_Update(void);
void AIZSetup_LateUpdate(void);
void AIZSetup_StaticUpdate(void);
void AIZSetup_Draw(void);
void AIZSetup_Create(void* data);
void AIZSetup_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void AIZSetup_EditorDraw(void);
void AIZSetup_EditorLoad(void);
#endif
void AIZSetup_Serialize(void);

// Extra Entity Functions
#if RETRO_USE_PLUS
void AIZSetup_BGSwitchCB_Jungle(void);
void AIZSetup_BGSwitchCB_Sky(void);
#endif

void AIZSetup_PlayerState_Static(void);
void AIZSetup_PlayerState_P2Enter(void);

void AIZSetup_HandleHeavyMovement(void);

void AIZSetup_SetupObjects(void);

void AIZSetup_GetCutsceneSetupPtr(void);
#if RETRO_USE_PLUS
void AIZSetup_SkipCB(void);
#endif

void AIZSetup_CutsceneST_Setup(void);
bool32 AIZSetup_CutsceneSonic_EnterAIZ(Entity *h);
bool32 AIZSetup_CutsceneSonic_EnterAIZJungle(Entity *h);
bool32 AIZSetup_CutsceneSonic_EnterHeavies(Entity *h);
bool32 AIZSetup_CutsceneSonic_P2FlyIn(Entity *h);
bool32 AIZSetup_CutsceneSonic_EnterClaw(Entity *h);
bool32 AIZSetup_CutsceneSonic_WatchClaw(Entity *h);
bool32 AIZSetup_CutsceneSonic_RubyGrabbed(Entity *h);
bool32 AIZSetup_CutsceneSonic_RubyAppear(Entity *h);
bool32 AIZSetup_CutsceneSonic_RubyFX(Entity *h);

void AIZSetup_CutsceneK_Setup(void);
bool32 AIZSetup_CutsceneKnux_Chillin(Entity *h);
bool32 AIZSetup_CutsceneKnux_StartDrillin(Entity *h);
bool32 AIZSetup_CutsceneKnux_Drillin(Entity *h);
bool32 AIZSetup_CutsceneKnux_PrepareForTrouble(Entity *h);
bool32 AIZSetup_CutsceneKnux_EnterThreat(Entity *h);
bool32 AIZSetup_CutsceneKnux_HeaviesAppear(Entity *h);
bool32 AIZSetup_CutsceneKnux_RubyImpact(Entity *h);
bool32 AIZSetup_CutsceneKnux_RubyFX(Entity *h);

bool32 AIZSetup_Cutscene_LoadGHZ(Entity *h);

#endif //!OBJ_AIZSETUP_H
