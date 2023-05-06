#ifndef OBJ_MSZSETUP_H
#define OBJ_MSZSETUP_H

#include "Game.h"

typedef enum { MSZ_GENERICTRIGGER_ACHIEVEMENT } GenericTriggerTypesMSZ;

typedef enum {
    MSZ_DECORATION_MAIN,
    MSZ_DECORATION_TANKERRAIL,
    MSZ_DECORATION_TANKERWHEEL,
    MSZ_DECORATION_CARRIAGEWHEEL,
    MSZ_DECORATION_CONNECTOR1,
    MSZ_DECORATION_CONNECTOR2,
} DecorationTypesMSZ;

typedef enum {
    MSZ_PARALLAXSPRITE_CACTUS1,
    MSZ_PARALLAXSPRITE_CACTUS2,
    MSZ_PARALLAXSPRITE_CACTUS3,
    MSZ_PARALLAXSPRITE_CACTUS4,
    MSZ_PARALLAXSPRITE_CACTUS5,
    MSZ_PARALLAXSPRITE_CACTUS6,
    MSZ_PARALLAXSPRITE_CACTUS7,
    MSZ_PARALLAXSPRITE_OOZPEEK,
} ParallaxSpriteAniIDsMSZ;

// Object Class
struct ObjectMSZSetup {
    RSDK_OBJECT
    int32 fadeTimer;
    uint16 aniTiles;
    TileLayer *background1;
    TABLE(int32 deformData[32], { -1, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
    STATIC(int32 bladeCactusFlowerAniTimer, 16);
    STATIC(int32 bobbingCactusFlowerAniTimer, 30);
    int32 bladeCactusFlowerAniFrame;
    int32 bobbingCactusFlowerAniFrame;
    int32 spinningCactusFlowerAniFrame;
    bool32 usingRegularPalette;
    TABLE(int32 bladeCactusFlowerAniSprPos[14], { 0, 1, 2, 3, 4, 5, 2, 3, 4, 5, 2, 1, 6, 7 });
    TABLE(int32 bladeCactusFlowerAniDuration[14], { 16, 2, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 2, 3 });
    TABLE(int32 bobbingCactusFlowerAniDuration[8], { 16, 2, 4, 5, 16, 2, 4, 5 });
    int32 storedParallax[512];
    int32 parallaxMult;
    int32 parallaxPos;
    bool32 hasAchievement;
    EntityMSZ2Cutscene *msz2Cutscene;
    int32 starPostID;
#if MANIA_USE_PLUS
    int32 chuggaVolume;
    int32 chuggaChannel;
    uint16 sfxLocoChugga;
#endif
};

// Entity Class
struct EntityMSZSetup {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
};

// Object Struct
extern ObjectMSZSetup *MSZSetup;

// Standard Entity Events
void MSZSetup_Update(void);
void MSZSetup_LateUpdate(void);
void MSZSetup_StaticUpdate(void);
void MSZSetup_Draw(void);
void MSZSetup_Create(void *data);
void MSZSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MSZSetup_EditorDraw(void);
void MSZSetup_EditorLoad(void);
#endif
void MSZSetup_Serialize(void);

// Extra Entity Functions
void MSZSetup_SetBGScrollOrigin(int32 x, int32 y);
void MSZSetup_StoreBGParallax(void);
void MSZSetup_ReloadBGParallax(void);
void MSZSetup_ReloadBGParallax_Multiply(int32 parallaxMult);

void MSZSetup_State_ManageFade_ST(void);
void MSZSetup_State_ManageFade_K(void);
#if MANIA_USE_PLUS
void MSZSetup_State_ManageFade_E(void);
#endif

void MSZSetup_StageFinish_EndAct1ST(void);
void MSZSetup_StageFinish_EndAct1K(void);
#if MANIA_USE_PLUS
void MSZSetup_StageFinish_EndAct1E(void);
#endif

void MSZSetup_StageFinish_EndAct2(void);
void MSZSetup_Trigger_AwardAchievement(void);
void MSZSetup_HandleRestart(void);
void MSZSetup_State_SwitchPalettes(void);

#if MANIA_USE_PLUS
void MSZSetup_State_CheckFadeTrigger_E(void);
void MSZSetup_State_CheckTrainStart(void);
void MSZSetup_State_TrainStarting(void);
void MSZSetup_State_TrainSequence_MSZ1E(void);
void MSZSetup_State_Boss_MSZ1E(void);
void MSZSetup_State_AwaitActClearStart(void);
void MSZSetup_State_AwaitActClearFinish(void);
void MSZSetup_State_MoveToMSZ2Start(void);
void MSZSetup_State_AwaitPlayerStopped(void);
void MSZSetup_State_StoreMSZ1ScrollPos_E(void);
#endif
void MSZSetup_State_StoreMSZ1ScrollPos_ST(void);

void MSZSetup_PlayerState_Pilot(void);
void MSZSetup_PlayerState_PostCrashJumpIn(void);

#endif //! OBJ_MSZSETUP_H
