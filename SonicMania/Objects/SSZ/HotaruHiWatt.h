#ifndef OBJ_HOTARUHIWATT_H
#define OBJ_HOTARUHIWATT_H

#include "Game.h"

typedef enum {
    HHW_BOSS,
    HHW_SINGLE_HOTARU,
    HHW_PAIR_HOTARU,
    HHW_UNUSED1,
    HHW_SMALL_HOTARU,
    HHW_FLASH,
    HHW_UNUSED2,
    HHW_MINILASER,
    HHW_ELECTRICORB,
} HHWTypes;

// Object Class
struct ObjectHotaruHiWatt {
    RSDK_OBJECT
    TABLE(int32 debrisInfo[33],
          { 8, 0, 0,        -0x20000, -0x28000, 1, 0,       0x20000,  -0x28000, 2, 0,        -0x28000, -0x20000, 3, 0,       0x28000, -0x20000,
            4, 0, -0x28000, -0x18000, 5,        0, 0x28000, -0x18000, 6,        0, -0x20000, -0x10000, 7,        0, 0x20000, -0x10000 });
    int32 boundsL;
    int32 boundsM;
    int32 boundsR;
    int32 boundsT;
    int32 boundsB;
    int32 startY;
    bool32 spawnedLaserStrike;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxHHWAppear;
    uint16 sfxFlash;
    uint16 sfxLaser;
    uint16 sfxAppear;
    uint16 sfxFly;
    uint16 sfxFlyUp;
    uint16 sfxCharge;
    uint16 aniFrames;
    uint16 hotaruFrames;
};

// Entity Class
struct EntityHotaruHiWatt {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(nextState);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 formationCircleRadius;
    int32 bulbAlpha;
    int32 unused;
    Vector2 originPos;
    Vector2 targetPos;
    Vector2 laserImpactPos;
    EntityHotaruHiWatt *parents[3];
    bool32 playingBossTrack;
    Animator mainAnimator;
    Animator headAnimator;
    Animator bulbAnimator;
    Animator bulbFlashAnimator;
    Animator laserImpactAnimator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectHotaruHiWatt *HotaruHiWatt;

// Standard Entity Events
void HotaruHiWatt_Update(void);
void HotaruHiWatt_LateUpdate(void);
void HotaruHiWatt_StaticUpdate(void);
void HotaruHiWatt_Draw(void);
void HotaruHiWatt_Create(void *data);
void HotaruHiWatt_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HotaruHiWatt_EditorDraw(void);
void HotaruHiWatt_EditorLoad(void);
#endif
void HotaruHiWatt_Serialize(void);

// Extra Entity Functions
bool32 HotaruHiWatt_SfxCheck_Zap(void);
bool32 HotaruHiWatt_SfxCheck_HHWLaser(void);
void HotaruHiWatt_SfxUpdate_HHWLaser(int32 sfx);

void HotaruHiWatt_Hit(void);
void HotaruHiWatt_Explode(void);

void HotaruHiWatt_CheckPlayerCollisions_Hotaru(void);
void HotaruHiWatt_CheckPlayerCollisions_Boss(void);
void HotaruHiWatt_CheckPlayerCollisions_MiniLaser(void);
void HotaruHiWatt_CheckPlayerCollisions_BossLaser(void);

// Drawing
void HotaruHiWatt_Draw_Hotaru(void);
void HotaruHiWatt_Draw_HotaruAttacking(void);
void HotaruHiWatt_Draw_Boss(void);
void HotaruHiWatt_Draw_BossLaserCharging(void);
void HotaruHiWatt_Draw_BossLaser(void);
void HotaruHiWatt_Draw_FormingHHW(void);
void HotaruHiWatt_Draw_FlashAppear(void);

// End of attack helpers
void HotaruHiWatt_SpawnPairHotarus(void);
void HotaruHiWatt_SetupHHWReappear(void);

// HotaruHiWatt Appear
void HotaruHiWatt_StateBoss_SetupArena(void);
void HotaruHiWatt_StateBoss_AwaitPlayer(void);
void HotaruHiWatt_StateBoss_Appear(void);
void HotaruHiWatt_StateBoss_FlyUp(void);

// Hotaru Misc
void HotaruHiWatt_StateHotaru_MoveToTarget(void);

void HotaruHiWatt_StateBoss_FlashFadeOut(void);

// Dimming/Undimming the screen
void HotaruHiWatt_StateBoss_HiddenDimScreen(void);
void HotaruHiWatt_StateBoss_HiddenUndimScreen(void);

// Single Hotaru
void HotaruHiWatt_StateHotaru_DimScreen(void);
void HotaruHiWatt_StateHotaru_Charging(void);
void HotaruHiWatt_StateHotaru_Attacking(void);
void HotaruHiWatt_StateHotaru_FinishedAttack(void);
void HotaruHiWatt_StateHotaru_EndAttackSequence(void);

// Single Hotaru (Laser)
void HotaruHiWatt_State_MiniLaser(void);
void HotaruHiWatt_State_MiniLaserStrike(void);

// Pair Hotarus
void HotaruHiWatt_StateHotaruPair_DimScreen(void);
void HotaruHiWatt_StateHotaruPair_PrepareAttack(void);
void HotaruHiWatt_StateHotaruPair_Charging(void);
void HotaruHiWatt_StateHotaruPair_AttackDelay(void);
void HotaruHiWatt_StateHotaruPair_AttackMovingDown(void);
void HotaruHiWatt_StateHotaruPair_AttackMovingUp(void);

// Sparky Orb
void HotaruHiWatt_State_SparkyOrb(void);
void HotaruHiWatt_State_SparkyOrb_FadeOut(void);

// HotaruHiWatt
void HotaruHiWatt_StateBoss_DimScreen(void);
void HotaruHiWatt_StateBoss_CreateSmallHHWs(void);
void HotaruHiWatt_StateBoss_FormingHHW(void);
void HotaruHiWatt_StateBoss_FlashAppear(void);
void HotaruHiWatt_StateBoss_LaserAttackDelay(void);
void HotaruHiWatt_StateBoss_ChargingLaser(void);
void HotaruHiWatt_StateBoss_LaserAttack_Right(void);
void HotaruHiWatt_StateBoss_LaserAttack_Left(void);
void HotaruHiWatt_StateBoss_FinishedLaserAttack(void);
void HotaruHiWatt_StateBoss_Destroyed(void);
void HotaruHiWatt_StateBoss_Finished(void);

#endif //! OBJ_HOTARUHIWATT_H
