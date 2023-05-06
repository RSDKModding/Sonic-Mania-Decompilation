#ifndef OBJ_DDWRECKER_H
#define OBJ_DDWRECKER_H

#include "Game.h"

typedef enum {
    DDWRECKER_SETUP,
    DDWRECKER_BALL1,
    DDWRECKER_BALL2,
    DDWRECKER_CHAIN,
    DDWRECKER_CORE,
} DDWreckerTypes;

// Object Class
struct ObjectDDWrecker {
    RSDK_OBJECT
    int32 camBoundL;
    int32 camBoundR;
    int32 bossBoundL;
    int32 bossBoundR;
    int32 bossBoundT;
    int32 attackVelocities[3];
    TABLE(int32 spinOffset[6], { -64, -32, -16, 16, 32, 64 });
    TABLE(int32 swingOffset[6], { 32, 48, 64, 80, 96, 128 });
    uint16 aniFrames;
    uint16 sfxBossHit;
    uint16 sfxExplosion;
    uint16 sfxDrop;
    uint16 sfxImpact;
    uint16 sfxAssemble;
    uint16 sfxRotate;
    uint16 sfxSharp;
};

// Entity Class
struct EntityDDWrecker {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateBall);
    Vector2 originPos;
    int32 type;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    bool32 swapBalls;
    int32 angleVel;
    int32 spinTimer;
    int32 spinAngle;
    int32 radius;
    uint16 siblingSlots[6];
    EntityDDWrecker *bodyA;
    EntityDDWrecker *bodyB;
    int32 unused1;
    int32 unusedAngle; // updated but never used..
    int32 unused2;
    int32 unused3;
    int32 blendAmount;
    Animator animator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectDDWrecker *DDWrecker;

// Standard Entity Events
void DDWrecker_Update(void);
void DDWrecker_LateUpdate(void);
void DDWrecker_StaticUpdate(void);
void DDWrecker_Draw(void);
void DDWrecker_Create(void *data);
void DDWrecker_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void DDWrecker_EditorDraw(void);
void DDWrecker_EditorLoad(void);
#endif
void DDWrecker_Serialize(void);

// Extra Entity Functions
void DDWrecker_State_SetupArena(void);
void DDWrecker_State_InitChildren(void);
void DDWrecker_State_Assemble(void);
void DDWrecker_State_EnterWreckers(void);
void DDWrecker_State_AttackDelay(void);
void DDWrecker_State_SwingRight(void);
void DDWrecker_State_SwingLeft(void);
void DDWrecker_State_SwingMoveToCenter(void);
void DDWrecker_State_HandleSpinning(void);
void DDWrecker_State_SwingSlowDown(void);
void DDWrecker_State_PrepareBounceAttack(void);
void DDWrecker_State_SignalBounceAttackStart(void);
void DDWrecker_State_HandleBounceAttack(void);
void DDWrecker_State_EndBounceAttack(void);

void DDWrecker_StateBall_Vulnerable(void);
void DDWrecker_StateBall_Spiked(void);
void DDWrecker_StateBall_Partnerless(void);

void DDWrecker_Hit(void);
void DDWrecker_Spin(void);
void DDWrecker_Swing(void);
void DDWrecker_Explode(void);

void DDWrecker_State_Debris(void);
void DDWrecker_State_Die(void);
void DDWrecker_State_SpawnSignpost(void);

#endif //! OBJ_DDWRECKER_H
