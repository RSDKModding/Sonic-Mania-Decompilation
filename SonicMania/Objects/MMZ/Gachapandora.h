#ifndef OBJ_GACHAPANDORA_H
#define OBJ_GACHAPANDORA_H

#include "Game.h"

#define GACHAPANDORA_CAPSULE_COUNT (4)

typedef enum {
    GACHAPANDORA_MAIN,
    GACHAPANDORA_EGGMAN,
    GACHAPANDORA_PRIZE,
    GACHAPANDORA_AMY,
    GACHAPANDORA_DRILLER,
    GACHAPANDORA_FIREDROPPER,
    GACHAPANDORA_FIREBALL,
    GACHAPANDORA_DEBRIS,
    GACHAPANDORA_SPARK,
} GachapandoraTypes;

// Object Class
struct ObjectGachapandora {
    RSDK_OBJECT
    TABLE(int32 capsuleOffsets[56],
          { 37, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 49, 50, 50, 50, 51, 51, 51, 51, 52, 52, 52, 52, 53, 53, 53, 53,
            54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 53, 53, 53, 52, 52, 52, 52, 51, 51, 51, 50, 50, 50, 50, 49, 49, 48, 48 });
    uint8 destroyedToys;
    uint8 handleDir;
    uint8 eggmanDir;
    uint8 capsuleOffsetL[4];
    uint8 capsuleSpeedL[4];
    uint8 capsuleMaxL[4];
    uint8 capsuleTypeL[4];
    uint8 capsuleDelayL[4];
    uint8 capsuleOffsetR[4];
    uint8 capsuleSpeedR[4];
    uint8 capsuleMaxR[4];
    uint8 capsuleTypeR[4];
    uint8 capsuleDelayR[4];
    int16 handleSpinTimer;
    uint8 nextPrizeType;
    int8 nextCapsuleL;
    int8 nextCapsuleR;
    int8 lastCapsuleType;
    uint8 capsuleCount[3];
    uint8 capsuleSide;
    uint8 debrisTimer;
    uint8 sideSparkTimer[2];
    int32 unused1;
    int32 unused2;
    uint8 activeToys;
    uint8 awardAchievement;
    EntityGachapandora *eggman;
    uint16 aniFrames;
    uint16 eggmanFrames;
    Hitbox hitboxHandle;
    Hitbox hitboxAmyRange;
    Hitbox hitboxAmy;
    Hitbox hitboxFireball;
    Hitbox hitboxDrill;
    Hitbox hitboxSpark;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxUnravel;
    uint16 sfxPon;
    uint16 sfxFireball;
    uint16 sfxGiggle;
};

// Entity Class
struct EntityGachapandora {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 unused;
    uint8 type;
    uint16 shakeTimer;
    uint16 shakeCount;
    uint16 prevShakeFlags;
    int32 timer;
    int32 startY;
    int32 invincibilityTimer;
    uint8 health;
    Vector2 originPos;
    EntityGachapandora *parent;
    Animator mainAnimator;
    Animator eggmanAnimator;
    Animator handleAnimator;
    Animator capsuleAnimator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectGachapandora *Gachapandora;

// Standard Entity Events
void Gachapandora_Update(void);
void Gachapandora_LateUpdate(void);
void Gachapandora_StaticUpdate(void);
void Gachapandora_Draw(void);
void Gachapandora_Create(void *data);
void Gachapandora_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Gachapandora_EditorDraw(void);
void Gachapandora_EditorLoad(void);
#endif
void Gachapandora_Serialize(void);

// Extra Entity Functions
void Gachapandora_CheckPlayerCollisions_Prize(void);
void Gachapandora_Explode(int32 xMin, int32 xMax, int32 yMin, int32 yMax);

void Gachapandora_Player_StateInput_P1Grabbed(void);
void Gachapandora_Player_StateInput_P2PlayerGrabbed(void);
void Gachapandora_Player_StateInput_P2AIGrabbed(void);

void Gachapandora_HandleSparksAndDebris(void);
void Gachapandora_HandleAnimations(void);

void Gachapandora_StateBoss_SetupArena(void);
void Gachapandora_StateBoss_AwaitPlayer(void);
void Gachapandora_StateBoss_EnterEggman(void);
void Gachapandora_StateBoss_FloatAround(void);
void Gachapandora_StateBoss_HandleSpun(void);
void Gachapandora_StateBoss_Explode(void);
void Gachapandora_StateBoss_PanicFlee(void);
void Gachapandora_StateBoss_EnsureAllCapsulesDestroyed(void);
void Gachapandora_StateBoss_LastDitchAttack(void);
void Gachapandora_StateBoss_Defeated(void);
void Gachapandora_StateBoss_EggmanFallOut(void);
void Gachapandora_StateBoss_Finish(void);
void Gachapandora_Draw_Boss(void);
void Gachapandora_Draw_BossDestroyed(void);

void Gachapandora_StatePrize_CapsuleFall(void);
void Gachapandora_StatePrize_ExitCapsule(void);
void Gachapandora_StatePrize_Destroyed(void);
void Gachapandora_StatePrize_DrillerCapsuleExitBounce(void);
void Gachapandora_StatePrize_DrillerMove(void);
void Gachapandora_StatePrize_FireDropperMove(void);
void Gachapandora_StatePrize_AmyIdle(void);
void Gachapandora_StatePrize_AmyWalk(void);
void Gachapandora_StatePrize_AmyJump(void);
void Gachapandora_StatePrize_AmyRebound(void);
void Gachapandora_StatePrize_AmyGrabbed(void);
void Gachapandora_Draw_Prize(void);

void Gachapandora_StateFireball_Falling(void);
void Gachapandora_StateFireball_BurnGround(void);

void Gachapandora_Draw_Simple(void);

void Gachapandora_StateDebris_Delay(void);
void Gachapandora_StateDebris_Falling(void);
void Gachapandora_StateDebris_BossDebris(void);
void Gachapandora_Draw_BossDebris(void);

void Gachapandora_StateSpark_Attatched(void);
void Gachapandora_StateSpark_Detatched(void);

void Gachapandora_StateEggman_Falling(void);
void Gachapandora_StateEggman_Escape(void);
void Gachapandora_StateEggman_RunAway(void);

#endif //! OBJ_GACHAPANDORA_H
