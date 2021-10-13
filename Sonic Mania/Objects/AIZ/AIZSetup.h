#ifndef OBJ_AIZSETUP_H
#define OBJ_AIZSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
#if RETRO_USE_PLUS
    int aniTiles1[13];
    int aniTiles2[12];
    int aniTiles3[15];
    int aniTiles4[14];
    int aniTiles5[15];
    int aniTiles6[14];
#endif
    bool32 cutsceneInit;
    int dword154;
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
    int aniTileFrameA;
    int aniTilesDelay1;
    int aniTileFrameB;
    int aniTilesDelay2;
    int aniTileFrameC;
    int aniTilesDelay3;
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
void AIZSetup_EditorDraw(void);
void AIZSetup_EditorLoad(void);
void AIZSetup_Serialize(void);

// Extra Entity Functions
#if RETRO_USE_PLUS
void AIZSetup_bgSwitch1_CB(void);
void AIZSetup_bgSwitch2_CB(void);
#endif

void AIZSetup_Unknown4(void);
void AIZSetup_Unknown5(void);
void AIZSetup_Unknown24(void);

void AIZSetup_SetupObjects(void);

void AIZSetup_GetCutsceneSetupPtr(void);
void AIZSetup_SkipCB(void);

void AIZSetup_CutsceneST_Setup(void);
bool32 AIZSetup_Cutscene1_Unknown1(Entity *h);
bool32 AIZSetup_Cutscene1_Unknown2(Entity *h);
bool32 AIZSetup_Cutscene1_Unknown3(Entity *h);
bool32 AIZSetup_Cutscene1_Unknown4(Entity *h);
bool32 AIZSetup_Cutscene1_Unknown5(Entity *h);
bool32 AIZSetup_Cutscene1_Unknown6(Entity *h);
bool32 AIZSetup_Cutscene1_Unknown7(Entity *h);
bool32 AIZSetup_Cutscene1_Unknown8(Entity *h);
bool32 AIZSetup_Cutscene1_Unknown9(Entity *h);

void AIZSetup_CutsceneK_Setup(void);
bool32 AIZSetup_Cutscene2_Unknown1(Entity *h);
bool32 AIZSetup_Cutscene2_Unknown2(Entity *h);
bool32 AIZSetup_Cutscene2_Unknown3(Entity *h);
bool32 AIZSetup_Cutscene2_Unknown4(Entity *h);
bool32 AIZSetup_Cutscene2_Unknown5(Entity *h);
bool32 AIZSetup_Cutscene2_Unknown6(Entity *h);
bool32 AIZSetup_Cutscene2_Unknown7(Entity *h);
bool32 AIZSetup_Cutscene2_Unknown8(Entity *h);

bool32 AIZSetup_Cutscene_LoadGHZ(Entity *h);

#endif //!OBJ_AIZSETUP_H
