#ifndef OBJ_OOZSETUP_H
#define OBJ_OOZSETUP_H

#include "Game.h"

typedef enum { OOZ_WARPDOOR_EFFECT_TO_SUB = 1, OOZ_WARPDOOR_EFFECT_FROM_SUB = 2 } OOZWarpDoorEffects;
typedef enum { OOZSETUP_FLAMES, OOZSETUP_FADE } OOZSetupTypes;

typedef enum { OOZ_TFLAGS_NORMAL, OOZ_TFLAGS_OILPOOL, OOZ_TFLAGS_OILSTRIP, OOZ_TFLAGS_OILSLIDE, OOZ_TFLAGS_OILFALL } TileFlagsOOZ;

typedef enum { OOZ_GENERICTRIGGER_ACHIEVEMENT } GenericTriggerTypesOOZ;

// Object Class
struct ObjectOOZSetup {
    RSDK_OBJECT
    int32 palTimer;
    uint16 aniTiles;
    TileLayer *background1;
    TABLE(int32 deformData[64], { 1, 2, 1, 3, 1, 2, 2, 1, 2, 3, 1, 2, 1, 2, 0, 0, 2, 0, 3, 2, 2, 3, 2, 2, 1, 3, 0, 0, 1, 0, 1, 3,
                                  1, 2, 1, 3, 1, 2, 2, 1, 2, 3, 1, 2, 1, 2, 0, 0, 2, 0, 3, 2, 2, 3, 2, 2, 1, 3, 0, 0, 1, 0, 1, 3 });
    int32 oilAniTimer;
    int32 glowLightAniFrame;
    STATIC(int32 glowLightAniTimer, 60);
    TABLE(int32 glowLightAniDurations[9], { 60, 60, 3, 3, 3, 3, 3, 3, 4 });
    int32 swimmingPlayerCount;
    int32 smogTimer;
    int32 useSmogEffect;
    uint8 flameTimers[0x20000];
    uint8 *flameTimerPtrs[400];
    Vector2 flamePositions[400];
    uint16 flameCount;
    uint8 activePlayers;
#if MANIA_USE_PLUS
    Animator flameAnimator;
    Animator animator;
    uint16 solFrames;
    uint16 splashFrames;
#else
    Animator flameAnimator;
    uint16 solFrames;
#endif
    bool32 hasAchievement;
    Entity *cutscenePtr;
};

// Entity Class
struct EntityOOZSetup {
    RSDK_ENTITY
    uint8 type;
};

// Object Struct
extern ObjectOOZSetup *OOZSetup;

// Standard Entity Events
void OOZSetup_Update(void);
void OOZSetup_LateUpdate(void);
void OOZSetup_StaticUpdate(void);
void OOZSetup_Draw(void);
void OOZSetup_Create(void *data);
void OOZSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void OOZSetup_EditorDraw(void);
void OOZSetup_EditorLoad(void);
#endif
void OOZSetup_Serialize(void);

// Extra Entity Functions
bool32 OOZSetup_SfxCheck_Flame2(void);
bool32 OOZSetup_SfxCheck_Slide(void);
bool32 OOZSetup_SfxCheck_OilSwim(void);

void OOZSetup_Draw_Flames(void);
void OOZSetup_HandleActiveFlames(void);
bool32 OOZSetup_StartFire(int32 posX, int32 posY, int32 angle);

void OOZSetup_Trigger_AwardAchievement(void);

void OOZSetup_PlayerState_OilPool(void);
void OOZSetup_PlayerState_OilStrip(void);
void OOZSetup_PlayerState_OilSlide(void);
void OOZSetup_PlayerState_OilFall(void);

#endif //! OBJ_OOZSETUP_H
