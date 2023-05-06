#ifndef OBJ_HEAVYGUNNER_H
#define OBJ_HEAVYGUNNER_H

#include "Game.h"

typedef enum {
    HEAVYGUNNER_HBH,
    HEAVYGUNNER_1,
    HEAVYGUNNER_HELI,
    HEAVYGUNNER_EGGROBO,
    HEAVYGUNNER_GUN,
    HEAVYGUNNER_MISSILE_F,
    HEAVYGUNNER_MISSILE,
    HEAVYGUNNER_ESCAPE_HBH,
} HeavyGunnerTypes;

// Object Class
struct ObjectHeavyGunner {
    RSDK_OBJECT
    int32 boundsL;
    int32 boundsR;
    int32 boundsT;
    int32 boundsB;
    bool32 stageWrapActive;
    Hitbox hitboxMissileF;
    Hitbox hitboxMissile;
    uint16 aniFrames;
    uint16 sfxFire;
    uint16 sfxJet;
    uint16 sfxHit;
    uint16 sfxDestroy;
    uint16 sfxExplosion2;
    uint16 sfxExplosion3;
    uint16 sfxCharge;
    uint16 sfxThrow;
    uint16 sfxFlyIn;
    uint16 sfxWooshIn;
    uint16 sfxWooshOut;
    uint16 sfxFlip;
    uint16 sfxRumble;
    int32 unused1;
    int32 unused2;
    int32 unused3;
    int32 unused4;
};

// Entity Class
struct EntityHeavyGunner {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    EntityHeavyGunner *parent;
    int32 timer;
    int32 flyInTimer;
    int32 parentOffset;
    int32 targetScale;
    int32 nextRoboID;
    int32 type;
    int32 missileID;
    bool32 defeated; // prolly unused, this is set only after the entity is destroyed so...
    Animator mainAnimator;
    Animator pilotAnimator;
    Animator frontAnimator;
    Animator tBladeAnimator;
    Animator rBladeAnimator;
    Animator beaconAnimator;
    Animator feetAnimator;
    Animator feet2Animator;
    Animator gunnerAnimator;
    Animator armAnimator;
    Animator exhaustAnimator;
};

// Object Struct
extern ObjectHeavyGunner *HeavyGunner;

// Standard Entity Events
void HeavyGunner_Update(void);
void HeavyGunner_LateUpdate(void);
void HeavyGunner_StaticUpdate(void);
void HeavyGunner_Draw(void);
void HeavyGunner_Create(void *data);
void HeavyGunner_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HeavyGunner_EditorDraw(void);
void HeavyGunner_EditorLoad(void);
#endif
void HeavyGunner_Serialize(void);

// Extra Entity Functions
bool32 HeavyGunner_SfxCheck_HeliProp(void);
bool32 HeavyGunner_SfxCheck_HBHSurprise(void);
bool32 HeavyGunner_SfxCheck_RocketBurn(void);

void HeavyGunner_HandleBGWrap(int32 multiplier);
void HeavyGunner_DestroyAllMissiles(void);

void HeavyGunner_Draw_Heli(void);
void HeavyGunner_Draw_EggRobo(void);
void HeavyGunner_Draw_Missile(void);
void HeavyGunner_Draw_Simple(void);
void HeavyGunner_Draw_FadeOut(void);

void HeavyGunner_StateManager_SetupArena(void);
void HeavyGunner_StateManager_HandleStageWrap(void);
void HeavyGunner_StateManager_HandlePathChange(void);

void HeavyGunner_Input_LockedP1(void);
void HeavyGunner_Input_LockedP2(void);
void HeavyGunner_Input_LockedP2_AI(void);

void HeavyGunner_StateEggRobo_Patrolling(void);
void HeavyGunner_StateEggRobo_FlyIn(void);
void HeavyGunner_StateEggRobo_ThrowGun(void);
void HeavyGunner_StateEggRobo_ThrownGun(void);

void HeavyGunner_StateGun_Thrown(void);
void HeavyGunner_StateGun_Grabbed(void);

void HeavyGunner_StateMissile_Launched(void);
void HeavyGunner_StateMissile_BlastOff(void);
void HeavyGunner_StateMissile_FindFloor(void);
void HeavyGunner_StateMissile_AttackPlayer(void);
void HeavyGunner_StateMissile_Malfunction(void);
void HeavyGunner_StateMissile_ReturnToSender(void);
void HeavyGunner_StateMissile_AttackRobo(void);
void HeavyGunner_StateMissile_AttackGunner(void);

void HeavyGunner_StateHeli_AwaitPlayer(void);
void HeavyGunner_StateHeli_WooshIn(void);
void HeavyGunner_StateHeli_FindFloor(void);
void HeavyGunner_StateHeli_HandleAttacks(void);
void HeavyGunner_StateHeli_ShotsFired(void);
void HeavyGunner_StateHeli_EscapeMissile(void);
void HeavyGunner_StateHeli_IncomingMissile(void);
void HeavyGunner_StateHeli_Exploding(void);
void HeavyGunner_StateHeli_ExplodeAndFall(void);

void HeavyGunner_StateEscapeHBH_Hover(void);
void HeavyGunner_StateEscapeHBH_FlyAway(void);

void HeavyGunner_StateHeli_FadeOutDestroy(void);

#endif //! OBJ_HEAVYGUNNER_H
