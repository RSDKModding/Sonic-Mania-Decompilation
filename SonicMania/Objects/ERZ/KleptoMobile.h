#ifndef OBJ_KLEPTOMOBILE_H
#define OBJ_KLEPTOMOBILE_H

#include "Game.h"

typedef enum {
    KLEPTOMOBILE_EGGMAN,
    KLEPTOMOBILE_ARM_L,
    KLEPTOMOBILE_ARM_R,
    KLEPTOMOBILE_HAND,
} KleptoMobileTypes;

// Object Class
struct ObjectKleptoMobile {
    RSDK_OBJECT
    TABLE(int32 debrisInfo[29], { 4,        0, 0, -0x20000, -0x28000, 1, 0, -0x20000, -0x28000, 2, 0, 0x20000, -0x20000, 3, 0, 0x28000,
                                  -0x20000, 4, 0, -0x10000, -0x10000, 5, 0, 0x10000,  -0x10000, 6, 0, -0x8000, -0x10000 });
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxFlail;
    uint16 sfxWhack;
    uint16 sfxPowerUp;
    uint16 sfxRocketJet;
    uint16 aniFrames;
    bool32 defeated;
};

// Entity Class
struct EntityKleptoMobile {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    Vector2 originPos;
    int32 armAngles[10]; // array of angles for the arm nodes, not actually used lol, possibly leftover from ERZ/PhantomKing?
    Vector2 armPositions[10];
    Vector2 armBezierPos;
    Vector2 bashArmTargetPos;
    Vector2 bashArmStartPos;
    int32 attackCount;
    int32 bashArmDelay;
    int32 bashArmID;
    bool32 canBashAttack;
    Vector2 rubyPos;
    bool32 holdingRuby;
    int32 armAngle;
    int32 circleRadius;
    int32 explosionVolume;
    EntityKleptoMobile *parent;
    Animator basicAnimator; // not actually used it seems
    Animator mobileTopAnimator;
    Animator eggmanAnimator;
    Animator mobileAnimator;
    Animator unusedAnimator1;
    Animator unusedAnimator2;
    Animator orbAnimator;
    Animator handAnimator;
    Animator finger1Animator;
    Animator finger2Animator;
    Animator rubyAnimator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectKleptoMobile *KleptoMobile;

// Standard Entity Events
void KleptoMobile_Update(void);
void KleptoMobile_LateUpdate(void);
void KleptoMobile_StaticUpdate(void);
void KleptoMobile_Draw(void);
void KleptoMobile_Create(void *data);
void KleptoMobile_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void KleptoMobile_EditorDraw(void);
void KleptoMobile_EditorLoad(void);
#endif
void KleptoMobile_Serialize(void);

// Extra Entity Functions
void KleptoMobile_HandleAnimations(void);
void KleptoMobile_CheckPlayerCollisions(void);
void KleptoMobile_Hit(void);
void KleptoMobile_Explode(void);
void KleptoMobile_HandleFrames(void);
void KleptoMobile_SwitchToKing(void);

void KleptoMobile_Draw_KleptoMobile(void);
void KleptoMobile_State_SetupArena(void);
void KleptoMobile_State_CutsceneControlled(void);
void KleptoMobile_State_MoveAround(void);
void KleptoMobile_State_Hover(void);
void KleptoMobile_HandleArmPositions(void);
void KleptoMobile_HandleChargeFinish(void);
void KleptoMobile_State_FirstChargeAttack(void);
void KleptoMobile_State_NextChargeAttacks(void);
void KleptoMobile_State_Switch(void);
void KleptoMobile_State_HitFall(void);

void KleptoMobile_StateHand_Cutscene(void);
void KleptoMobile_StateHand_Boss(void);
void KleptoMobile_Draw_Hand(void);

void KleptoMobile_CheckPlayerCollisions_Arm(void);

void KleptoMobile_StateArm_Cutscene(void);
void KleptoMobile_StateArm_Idle(void);
void KleptoMobile_StateArm_BashAttack(void);
void KleptoMobile_StateArm_ChargeAttack(void);
void KleptoMobile_Draw_Arm(void);

void KleptoMobile_State_Destroyed(void);
void KleptoMobile_State_Explode(void);
void KleptoMobile_State_CutsceneExplode(void);

#endif //! OBJ_KLEPTOMOBILE_H
