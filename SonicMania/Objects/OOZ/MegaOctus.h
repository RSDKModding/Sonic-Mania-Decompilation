#ifndef OBJ_MEGAOCTUS_H
#define OBJ_MEGAOCTUS_H

#include "Game.h"

typedef enum {
    MEGAOCTUS_BODY,
    MEGAOCTUS_UNUSED1,
    MEGAOCTUS_HARPOON,
    MEGAOCTUS_CANNON,
    MEGAOCTUS_ORB,
    MEGAOCTUS_ARM,
    MEGAOCTUS_LASER,
    MEGAOCTUS_ORBSHOT,
    MEGAOCTUS_UNUSED8,
    MEGAOCTUS_LASERFIRE,
} MegaOctusTypes;

// Object Class
struct ObjectMegaOctus {
    RSDK_OBJECT
    EntityMegaOctus *bossEntity;
    Animator noseAnimator;
    Animator boltsAnimator;
    Animator eggmanAnimator;
    Animator hatchBaseAnimator;
    Animator hatchOpenAnimator;
    int32 eggmanOffset;
    int32 eggmanVelocity;
    bool32 spawnHarpoon;
    int32 turnPos;
    TABLE(int32 turnOffsets[11], { -0x200000, -0x180000, -0x100000, -0xC0000, -0x80000, 0, 0x80000, 0xC0000, 0x100000, 0x180000, 0x200000 });
    TABLE(int32 cannonHeights[4], { -0x780000, -0x800000, -0x700000, -0x510000 });
    int32 orbHealth[2];
    bool32 defeated;
    uint16 aniFrames;
    uint16 eggmanFrames;
    uint16 hatchFrames;
    Hitbox hitbox;
    uint16 sfxBossHit;
    uint16 sfxExplosion;
    uint16 sfxLaser;
    uint16 sfxBullet;
    uint16 sfxHarpoon;
    uint16 sfxSurface;
    uint16 sfxLaserSplash;
};

// Entity Class
struct EntityMegaOctus {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    uint8 type;
    int32 timer;
    int32 orbMoveAngle;
    int32 targetPos;
    int32 lastAttackHeight;
    int32 shotCount;
    int32 invincibilityTimer;
    uint8 health;
    Vector2 origin;
    int32 orbID;
    Entity *parent;
#if MANIA_USE_PLUS
    int32 tilePlatY;
#endif
    Animator animator;
    Animator altAnimator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectMegaOctus *MegaOctus;

// Standard Entity Events
void MegaOctus_Update(void);
void MegaOctus_LateUpdate(void);
void MegaOctus_StaticUpdate(void);
void MegaOctus_Draw(void);
void MegaOctus_Create(void *data);
void MegaOctus_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MegaOctus_EditorDraw(void);
void MegaOctus_EditorLoad(void);
#endif
void MegaOctus_Serialize(void);

// Extra Entity Functions
void MegaOctus_CheckPlayerCollisions_Body(void);
void MegaOctus_HandleEggmanAnim(void);
void MegaOctus_Hit(void);
void MegaOctus_Explode(void);
void MegaOctus_HandleDirectionChange(void);

void MegaOctus_State_SetupBounds(void);
void MegaOctus_State_SetupArena(void);
void MegaOctus_State_None(void);
void MegaOctus_State_EnterMegaOctus(void);
void MegaOctus_State_OpenHatchAndLaugh(void);
void MegaOctus_State_CloseHatch(void);
void MegaOctus_State_DiveIntoOil(void);
void MegaOctus_State_SpawnWeapons(void);
void MegaOctus_State_CannonThenSpawnOrbs(void);
void MegaOctus_State_Destroyed(void);
void MegaOctus_State_Finish(void);
void MegaOctus_Draw_Body(void);

void MegaOctus_CheckPlayerCollisions_Harpoon(void);
void MegaOctus_State_HarpoonLeft(void);
void MegaOctus_State_HarpoonRight(void);
void MegaOctus_Draw_HarpoonLeft(void);
void MegaOctus_Draw_HarpoonRight(void);

void MegaOctus_CheckPlayerCollisions_Cannon(void);
void MegaOctus_StateCannon_RiseUp(void);
void MegaOctus_StateCannon_Idle(void);
void MegaOctus_StateCannon_FireLaser(void);
void MegaOctus_StateCannon_SinkDown(void);
void MegaOctus_Draw_Cannon(void);

void MegaOctus_CheckPlayerCollisions_Orb(void);
void MegaOctus_StateOrb_Wait(void);
void MegaOctus_StateOrb_FireShot(void);
void MegaOctus_StateOrb_Idle(void);
void MegaOctus_StateOrb_Destroyed(void);
void MegaOctus_Draw_Orb(void);

void MegaOctus_StateArm_WrapAroundPlatform(void);
void MegaOctus_StateArm_GrabbedPlatform(void);
void MegaOctus_StateArm_PullPlatformDown(void);
void MegaOctus_StateArm_RisePlatformUp(void);
void MegaOctus_Draw_Arm_WrapAroundPlatformBase(void);

void MegaOctus_StateArm_GrabPlatform(void);
void MegaOctus_Draw_Arm_WrapAroundPlatformTop(void);

void MegaOctus_State_Laser(void);

void MegaOctus_State_LaserFire(void);
void MegaOctus_Draw_Laser(void);

void MegaOctus_State_Shot(void);
void MegaOctus_Draw_OrbShot(void);

#if MANIA_USE_PLUS
void MegaOctus_TilePlatformState_RiseOuttaOil(void);
#endif

#endif //! OBJ_MEGAOCTUS_H
