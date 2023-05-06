#ifndef OBJ_PHANTOMEGG_H
#define OBJ_PHANTOMEGG_H

#include "Game.h"

typedef enum {
    PHANTOMEGG_EGGMAN,
} PhantomEggTypes;

// Names for the values in the attack tables
// Tables can only have int constants so we cant put these in there directly
typedef enum {
    PHANTOMEGG_ATTACK_NONE,
    PHANTOMEGG_ATTACK_JUMP,
    PHANTOMEGG_ATTACK_SHOCK,
    PHANTOMEGG_ATTACK_MISSILES,
    PHANTOMEGG_ATTACK_WARP,
} PhantomEggAttackIDs;

// Object Class
struct ObjectPhantomEgg {
    RSDK_OBJECT
    TABLE(int32 debrisInfo[29], { 4,        0, 0, -0x20000, -0x28000, 1, 0, -0x20000, -0x28000, 2, 0, 0x20000, -0x20000, 3, 0, 0x28000,
                                  -0x20000, 4, 0, -0x10000, -0x10000, 5, 0, 0x10000,  -0x10000, 6, 0, -0x8000, -0x10000 });
    TABLE(int32 attackStateTable1[32], { 0, 0, 0, 0, 1, 0, 0, 2, 0, 1, 3, 0, 2, 0, 0, 4, 0, 0, 1, 0, 2, 0, 0, 3, 0, 0, 1, 0, 0, 2, 0, 4 });
    TABLE(int32 attackStateTable2[32], { 0, 0, 2, 0, 1, 3, 0, 2, 0, 0, 3, 0, 2, 0, 0, 3, 0, 0, 0, 1, 2, 0, 0, 3, 0, 0, 1, 0, 0, 2, 0, 4 });
    TABLE(int32 attackStateTable3[32], { 0, 0, 2, 0, 1, 3, 3, 2, 0, 3, 3, 0, 2, 0, 0, 3, 0, 0, 0, 1, 2, 0, 0, 3, 0, 0, 1, 0, 0, 2, 0, 4 });
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    int32 boundsStoreL1;
    int32 boundsStoreR1;
    int32 boundsStoreT1;
    int32 boundsStoreB1;
    int32 startScanline;
    int32 endScanline;
    bool32 setupPalette;
    bool32 savedGameProgress;
    uint16 sfxHit;
    uint16 sfxExplosion2;
    uint16 sfxRocketJet;
    uint16 sfxExplosion3;
    uint16 sfxJump;
    uint16 sfxLand;
    uint16 sfxRepel;
    uint16 sfxShield;
    uint16 sfxShock;
    uint16 sfxSummon;
    uint16 sfxMissile;
    uint16 aniFrames;
    bool32 disableSuperForm;
};

// Entity Class
struct EntityPhantomEgg {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 remainingJumps;
    int32 attackTimer;
    int32 phantomID;
    int32 palBlendPercent;
    Vector2 targetPos;
    Vector2 targetVelocity;
    int32 *attackStateTable;
    int32 unused;
    Animator crackAnimator;
    Animator coreAnimator;
    Animator legAnimator;
    Animator armLAnimator;
    Animator armRAnimator;
    Animator eggmanAnimator;
    Animator rubyAnimator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectPhantomEgg *PhantomEgg;

// Standard Entity Events
void PhantomEgg_Update(void);
void PhantomEgg_LateUpdate(void);
void PhantomEgg_StaticUpdate(void);
void PhantomEgg_Draw(void);
void PhantomEgg_Create(void *data);
void PhantomEgg_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PhantomEgg_EditorDraw(void);
void PhantomEgg_EditorLoad(void);
#endif
void PhantomEgg_Serialize(void);

// Extra Entity Functions
void PhantomEgg_HandleAnimations(void);
void PhantomEgg_CheckPlayerCollisions(void);
void PhantomEgg_Hit(void);
void PhantomEgg_Explode(Hitbox *hitbox);
void PhantomEgg_HandleNextAttack(void);
void PhantomEgg_SetupWarpFX(void);
void PhantomEgg_HandlePhantomWarp(uint8 phantomID);
void PhantomEgg_HandleReturnWarp(void);
void PhantomEgg_Scanline_WarpFX(ScanlineInfo *scanlines);

void PhantomEgg_Draw_Normal(void);
void PhantomEgg_Draw_Cracked(void);

void PhantomEgg_State_SetupArena(void);
void PhantomEgg_State_DimArena(void);
void PhantomEgg_State_EnterEggman(void);
void PhantomEgg_State_AdjustStartingPos(void);
void PhantomEgg_State_IntroHover(void);
void PhantomEgg_State_BeginFight(void);
void PhantomEgg_State_MoveAround(void);
void PhantomEgg_State_Attack_Jumped(void);
void PhantomEgg_State_Attack_JumpLand(void);
void PhantomEgg_State_Attack_JumpAttack(void);
void PhantomEgg_State_Attack_CableShock(void);
void PhantomEgg_State_Attack_PrepareWarp(void);
void PhantomEgg_State_Attack_GrabPlayers(void);
void PhantomEgg_State_Attack_HandleWarp(void);
void PhantomEgg_State_Attack_WarpAway(void);
void PhantomEgg_State_Attack_WarpReturn(void);
void PhantomEgg_State_Destroyed(void);
void PhantomEgg_State_Exploding(void);
void PhantomEgg_State_StartBadEnd(void);
void PhantomEgg_State_CrackOpen(void);
void PhantomEgg_State_CrackedExploding(void);
void PhantomEgg_State_StartGoodEnd(void);

#if MANIA_USE_PLUS
void PhantomEgg_SaveGameCB(bool32 success);
#else
void PhantomEgg_SaveGameCB(void);
#endif

#endif //! OBJ_PHANTOMEGG_H
