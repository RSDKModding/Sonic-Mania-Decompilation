#ifndef OBJ_PHANTOMRIDER_H
#define OBJ_PHANTOMRIDER_H

#include "Game.h"

typedef enum { PHANTOMRIDER_JIMMY = 3 } PhantomRiderTypes;

// Object Class
struct ObjectPhantomRider {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxRiderLaunch;
    uint16 unused;
};

// Entity Class
struct EntityPhantomRider {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 startPos;
    int32 timer;
    int32 invincibilityTimer;
    int32 unused;
    int32 type;
    bool32 prevOnGround;
    EntityPhantomRider *child;
    Hitbox hitbox;
    Hitbox outerBox;
    Hitbox innerBox;
    Animator mainAnimator;
    Animator jimmyAnimator;
    Animator wheelAnimator;
    Animator thrustAnimator;
    Animator unusedAnimator1;
    Animator unusedAnimator2;
};

// Object Struct
extern ObjectPhantomRider *PhantomRider;

// Standard Entity Events
void PhantomRider_Update(void);
void PhantomRider_LateUpdate(void);
void PhantomRider_StaticUpdate(void);
void PhantomRider_Draw(void);
void PhantomRider_Create(void *data);
void PhantomRider_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PhantomRider_EditorDraw(void);
void PhantomRider_EditorLoad(void);
#endif
void PhantomRider_Serialize(void);

// Extra Entity Functions
void PhantomRider_ProcessAutoScroll(void);
void PhantomRider_Hit(void);

void PhantomRider_Draw_Rider(void);

void PhantomRider_State_HandleBegin(void);
void PhantomRider_State_EnterRider(void);
void PhantomRider_State_InitialRace(void);
void PhantomRider_State_RacePlayer(void);
void PhantomRider_State_ExitRider(void);

void PhantomRider_State_Jimmy(void);

#endif //! OBJ_PHANTOMRIDER_H
