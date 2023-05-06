#ifndef OBJ_LAUNDROMOBILE_H
#define OBJ_LAUNDROMOBILE_H

#include "Game.h"

typedef enum {
    LAUNDROMOBILE_BOSS,
    LAUNDROMOBILE_BOMB,
    LAUNDROMOBILE_LAUNDRY,
    LAUNDROMOBILE_LOOPPOINT,
    LAUNDROMOBILE_BLOCK,
    LAUNDROMOBILE_SPIKES,
    LAUNDROMOBILE_DELAYEDSPLASH,
} LaundroMobileTypes;

// Object Class
struct ObjectLaundroMobile {
    RSDK_OBJECT
    uint8 health;
    uint8 invincibilityTimer;
    int32 currentVelocity;
    Entity *loopPoints[6];
    uint8 nextLoopPoint;
    uint8 playingLoopSfx;
    uint8 loopSfxTimer;
    uint8 attackDir;
    int8 attackCount;
    TABLE(int32 attackCounts[8], { 1, 2, 2, 2, 1, 2, 2, 3 });
    uint8 isUnderwater;
    int32 animSpeed;
    uint8 rocketActive;
    int32 rocketAngles[4];
    Vector2 rocketPositions[4];
    uint8 unused[4]; // no clue, never even touched
    int32 rocketSpeeds[2];
    int32 playerRadius[PLAYER_COUNT];
    int32 playerAngles[PLAYER_COUNT];
    uint8 travelledPaths;
    uint8 useStageWrap;
    EntityLaundroMobile *laundroMobile;
    TABLE(int32 debrisInfo[73], { 12,       6,        0, -0x50000, -0x40000, 0, 0,        7,        0, 0x50000,  -0x40000, 0, 0,        10,       0,
                                  -0x50000, -0x40000, 0, 0,        11,       0, 0x50000,  -0x40000, 0, 0,        10,       0, -0x50000, -0x40000, 0,
                                  0x200000, 11,       0, 0x50000,  -0x40000, 0, 0x200000, 10,       0, -0x50000, -0x40000, 0, 0x400000, 11,       0,
                                  0x50000,  -0x40000, 0, 0x400000, 8,        0, -0x50000, -0x40000, 0, 0x400000, 9,        0, 0x50000,  -0x40000, 0,
                                  0x400000, 12,       0, -0x50000, -0x40000, 0, 0x400000, 13,       0, 0x50000,  -0x40000, 0, 0x400000 });
    uint16 aniFrames;
    uint16 eggmanFrames;
    Hitbox hitboxBoss;
    Hitbox hitboxMissile;
    Hitbox hitboxBox;
    Hitbox hitboxBomb;
    Hitbox innerBox;
    Hitbox outerBox;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxButton2;
    uint16 sfxFan;
    uint16 sfxRoll;
    uint16 sfxWash;
    uint16 sfxHullClose;
    uint16 sfxPush;
    uint16 sfxFireball;
    uint16 sfxButton;
    uint16 sfxLedgeBreak;
    uint16 sfxPimPom;
#if MANIA_USE_PLUS
    uint16 sfxRumble;
    uint16 sfxImpact;
#endif
};

// Entity Class
struct EntityLaundroMobile {
    RSDK_ENTITY
    uint8 type;
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer;
    int32 startY;
    Vector2 originPos;
    Vector2 unusedPos; // updated in the level wrap, but never used aside from that
    EntityWhirlpool *whirlpool;
    Animator mainAnimator;
    Animator propellerAnimator;
    Animator eggmanAnimator;
    Animator flameAnimator;
};

// Object Struct
extern ObjectLaundroMobile *LaundroMobile;

// Standard Entity Events
void LaundroMobile_Update(void);
void LaundroMobile_LateUpdate(void);
void LaundroMobile_StaticUpdate(void);
void LaundroMobile_Draw(void);
void LaundroMobile_Create(void *data);
void LaundroMobile_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LaundroMobile_EditorDraw(void);
void LaundroMobile_EditorLoad(void);
#endif
void LaundroMobile_Serialize(void);

// Extra Entity Functions
void LaundroMobile_CheckPlayerCollisions(void);
void LaundroMobile_Explode(void);
void LaundroMobile_HandleStageWrap(void);
void LaundroMobile_HandleRocketMovement(void);
void LaundroMobile_HandleEggmanAnimations(void);
void LaundroMobile_HandleTileCollisions(void);

// Phase 1
void LaundroMobile_StateBoss_AwaitPlayer_Phase1(void);
void LaundroMobile_StateBoss_SetupArena_Phase1(void);
void LaundroMobile_StateBoss_EnterEggman_Phase1(void);
void LaundroMobile_StateBoss_StartupPropellers(void);
void LaundroMobile_StateBoss_HandlePhase1(void);
void LaundroMobile_StateBoss_Destroyed_Phase1(void);
void LaundroMobile_StateBoss_Explode_Phase1(void);
void LaundroMobile_StateBoss_WaitForLastStageWrap(void);

// Phase 2
void LaundroMobile_StateBoss_AwaitPlayer_Phase2(void);
void LaundroMobile_StateBoss_SetupArena_Phase2(void);
void LaundroMobile_StateBoss_EnterEggman_Phase2(void);
void LaundroMobile_StateBoss_StartupRockets(void);
void LaundroMobile_StateBoss_SpeedUpRockets(void);
void LaundroMobile_StateBoss_RiseUpToAttack(void);
void LaundroMobile_StateBoss_Attacking(void);
void LaundroMobile_StateBoss_ReturnToLaundry(void);
void LaundroMobile_StateBoss_PrepareWhirlpool(void);
void LaundroMobile_StateBoss_StartupWhirlpool(void);
void LaundroMobile_StateBoss_WhirlpoolActive(void);
void LaundroMobile_StateBoss_PrepareRockets(void);
void LaundroMobile_StateBoss_Destroyed_Phase2(void);
void LaundroMobile_StateBoss_Explode_Phase2(void);

#if MANIA_USE_PLUS
void LaundroMobile_StageFinish_Wait(void);

// HCZ2 Outro
void LaundroMobile_StateOutro_StartCutscene(void);
void LaundroMobile_StateOutro_Rumble(void);
void LaundroMobile_StateOutro_WaterGush(void);
void LaundroMobile_StateOutro_ExitHCZ(void);
#endif

void LaundroMobile_Draw_Boss(void);
void LaundroMobile_Draw_Boss_Destroyed(void);

void LaundroMobile_StateBomb_Spawner(void);
void LaundroMobile_StateBomb_Bomb_Idle(void);
void LaundroMobile_StateBomb_Bomb_Activated(void);

void LaundroMobile_Draw_Simple(void);

void LaundroMobile_StateBlock_Spawner(void);
void LaundroMobile_StateBlock_Block(void);

void LaundroMobile_State_Laundry(void);
void LaundroMobile_Draw_Laundry(void);

void LaundroMobile_State_DelayedSplash(void);

#endif //! OBJ_LAUNDROMOBILE_H
