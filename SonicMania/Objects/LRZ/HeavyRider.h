#ifndef OBJ_HEAVYRIDER_H
#define OBJ_HEAVYRIDER_H

#include "Game.h"

typedef enum {
    HEAVYRIDER_RIDER,
    HEAVYRIDER_PLANESWITCH,
    HEAVYRIDER_FORCECHARGE_LEFT,
    HEAVYRIDER_FORCECHARGE_RIGHT,
    HEAVYRIDER_UNUSED1,
    HEAVYRIDER_UNUSED2,
    HEAVYRIDER_PUFF,
    HEAVYRIDER_FIREBALL,
    HEAVYRIDER_JIMMY,
} HeavyRiderTypes;

typedef enum {
    HEAVYRIDER_ATK_RIDING_SCREENBOUNDS,
    HEAVYRIDER_ATK_RIDING_ARENABOUNDS,
    HEAVYRIDER_ATK_RIDING_USETRIGGERS,
    HEAVYRIDER_ATK_PREPCHARGE,
    HEAVYRIDER_ATK_CHARGE,
} HeavyRiderAttackIDs;

typedef enum {
    HEAVYRIDER_SPIKEBALL_NONE,
    HEAVYRIDER_SPIKEBALL_SWINGING,
    HEAVYRIDER_SPIKEBALL_UNUSED1,
    HEAVYRIDER_SPIKEBALL_UNUSED2,
    HEAVYRIDER_SPIKEBALL_UNUSED3,
    HEAVYRIDER_SPIKEBALL_UNUSED4,
    HEAVYRIDER_SPIKEBALL_UNUSED5,
    HEAVYRIDER_SPIKEBALL_THROWN,
} HeavyRiderSpikeBallStates;

typedef enum {
    HEAVYRIDER_WHEEL_NONE,
    HEAVYRIDER_WHEEL_EXTENDING,
    HEAVYRIDER_WHEEL_RETRACTING,
} HeavyRiderWheelStates;

// Object Class
struct ObjectHeavyRider {
    RSDK_OBJECT
    uint8 curAttack;
    uint8 spikeBallState;
    Vector2 spikeBallPos;
    uint8 spikeBallAngle;
    int32 wheelLength;
    uint8 wheelExtendState;
    uint8 health;
    uint8 invincibilityTimer;
    uint8 spawnDelay;
    uint8 playerTimers[PLAYER_COUNT];
    int32 spawnFireballs;
    int32 startY;
    uint8 chargeTimer;
#if MANIA_USE_PLUS
    EntityLRZ3Outro *outro;
#endif
    uint16 aniFrames;
    Hitbox hitboxRider;
    Hitbox hitboxSpikeBall;
    Hitbox innerBox;
    Hitbox outerBox;
    Hitbox hitboxRiderTrigger;
    Hitbox hitboxTrigger;
    Hitbox hitboxJimmy;
    Hitbox hitboxFireball;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxCharge;
    uint16 sfxLaunch;
    uint16 sfxSkid;
    uint16 sfxJump;
    uint16 sfxCheer;
    uint16 sfxBumper;
};

// Entity Class
struct EntityHeavyRider {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    uint8 type;
    int32 flags;
    int32 size;
    bool32 affectPlayer;
    int32 timer;
    Animator mainAnimator;
    Animator thrustAnimator;
    Animator spikeBallAnimator;
    Animator fireballAnimator;
    Animator wheelieAnimator;
};

// Object Struct
extern ObjectHeavyRider *HeavyRider;

// Standard Entity Events
void HeavyRider_Update(void);
void HeavyRider_LateUpdate(void);
void HeavyRider_StaticUpdate(void);
void HeavyRider_Draw(void);
void HeavyRider_Create(void *data);
void HeavyRider_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HeavyRider_EditorDraw(void);
void HeavyRider_EditorLoad(void);
#endif
void HeavyRider_Serialize(void);

// Extra Entity Functions
void HeavyRider_SpawnDebris(int32 frame, uint8 drawGroup, int32 x, int32 y);
void HeavyRider_CheckObjectCollisions(void);
void HeavyRider_Hit(void);
void HeavyRider_Explode(void);
void HeavyRider_HandleTurn_ScreenEdges(void);
void HeavyRider_HandleTurn_ArenaEdges(void);
void HeavyRider_DecideNextAttack(void);

void HeavyRider_State_SetupArena(void);
void HeavyRider_State_AwaitPlayer(void);
void HeavyRider_State_SetupRider(void);
void HeavyRider_State_Moving(void);
void HeavyRider_State_RampJump(void);
void HeavyRider_State_Charging(void);
void HeavyRider_State_ChargeDash(void);
void HeavyRider_State_Turning(void);
void HeavyRider_State_Destroyed(void);
void HeavyRider_State_Finish(void);
void HeavyRider_Draw_Boss(void);

void HeavyRider_State_PlaneSwitch(void);
void HeavyRider_Draw_PlaneSwitch(void);

void HeavyRider_State_ChargeTrigger(void);

void HeavyRider_Draw_Simple(void);

void HeavyRider_State_Fireball(void);

void HeavyRider_State_Puff(void);

void HeavyRider_StateJimmy_Idle(void);
void HeavyRider_StateJimmy_RevRelease(void);
void HeavyRider_StateJimmy_RevUp(void);

#endif //! OBJ_HEAVYRIDER_H
