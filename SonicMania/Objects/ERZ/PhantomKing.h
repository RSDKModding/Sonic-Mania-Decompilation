#ifndef OBJ_PHANTOMKING_H
#define OBJ_PHANTOMKING_H

#include "Game.h"

typedef enum {
    PHANTOMKING_KING,
    PHANTOMKING_ARM_L,
    PHANTOMKING_ARM_R,
} PhantomKingTypes;

// Object Class
struct ObjectPhantomKing {
    RSDK_OBJECT
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 aniFrames;
    bool32 defeated;
};

// Entity Class
struct EntityPhantomKing {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 originPos;
    int32 type;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 typeChangeTimer; // Leftover from ERZKing, never used but the draw code for it is still there
    int32 bodyAngle;
    int32 armAngles[10];
    Vector2 armPositions[10];
    Vector2 armBezierPos;
    Vector2 rubyPos;
    Vector2 unused;
    bool32 drawRuby;
    bool32 canLaunchAttacks;
    EntityPhantomKing *parent;
    int32 armRadius;
    int32 armAngle;
    int32 armAngleOffset;
    Animator basicAnimator;
    Animator headAnimator;
    Animator bodyAnimator;
    Animator beltAnimator;
    Animator particleAnimator;
    Animator rubyAnimator;
    Animator unusedAnimator;
    Animator armAnimator;
    Animator cuffAnimator;
    Animator handAnimator;
    Hitbox hitbox;
    uint8 storeDrawFX;
    bool32 finishedMovingRuby;
};

// Object Struct
extern ObjectPhantomKing *PhantomKing;

// Standard Entity Events
void PhantomKing_Update(void);
void PhantomKing_LateUpdate(void);
void PhantomKing_StaticUpdate(void);
void PhantomKing_Draw(void);
void PhantomKing_Create(void *data);
void PhantomKing_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PhantomKing_EditorDraw(void);
void PhantomKing_EditorLoad(void);
#endif
void PhantomKing_Serialize(void);

// Extra Entity Functions
void PhantomKing_CheckPlayerCollisions(void);
void PhantomKing_Hit(void);
void PhantomKing_Explode(void);
void PhantomKing_HandleFrames(void);
void PhantomKing_HandleAttacks(void);
void PhantomKing_SwitchToEggman(void);
void PhantomKing_SetupKing(EntityPhantomKing *king);

void PhantomKing_Draw_Body(void);
void PhantomKing_Draw_Arm(void);

void PhantomKing_State_Initialize(void);
void PhantomKing_State_SetupArms(void);
void PhantomKing_State_EnterKing(void);
void PhantomKing_State_InitialHover(void);
void PhantomKing_State_TakeRubyAway(void);
void PhantomKing_State_RubyHoldHover(void);
void PhantomKing_State_WrestleEggman(void);
void PhantomKing_State_FlyAround(void);
void PhantomKing_State_Switch(void);
void PhantomKing_State_HitFall(void);

void PhantomKing_StateArm_Idle(void);
void PhantomKing_StateArm_WrestleEggman(void);
void PhantomKing_HandleArmMovement(void);
void PhantomKing_StateArm_PullBack(void);
void PhantomKing_StateArm_Point(void);

void PhantomKing_DestroyEntity(void);

void PhantomKing_State_Explode(void);
void PhantomKing_State_Destroyed(void);

#endif //! OBJ_PHANTOMKING_H
