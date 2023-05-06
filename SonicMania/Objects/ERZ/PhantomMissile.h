#ifndef OBJ_PHANTOMMISSILE_H
#define OBJ_PHANTOMMISSILE_H

#include "Game.h"

// Object Class
struct ObjectPhantomMissile {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
};

// Entity Class
struct EntityPhantomMissile {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 oscillateAngle;
    int32 targetRadius;
    int32 reattachRadius;
    int32 id;
    Vector2 targetPos;
    EntityPhantomEgg *parent;
    Animator missileAnimator;
    Animator targetOutsideAnimator;
    Animator targetInsideAnimator;
    Animator targetNumbersAnimator;
};

// Object Struct
extern ObjectPhantomMissile *PhantomMissile;

// Standard Entity Events
void PhantomMissile_Update(void);
void PhantomMissile_LateUpdate(void);
void PhantomMissile_StaticUpdate(void);
void PhantomMissile_Draw(void);
void PhantomMissile_Create(void *data);
void PhantomMissile_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PhantomMissile_EditorDraw(void);
void PhantomMissile_EditorLoad(void);
#endif
void PhantomMissile_Serialize(void);

// Extra Entity Functions
void PhantomMissile_GetTargetPos(void);
void PhantomMissile_HandleExhaust(void);

void PhantomMissile_State_Attached(void);
void PhantomMissile_State_PrepareLaunch(void);
void PhantomMissile_State_Launched(void);
void PhantomMissile_State_Attacking(void);
void PhantomMissile_State_Explode(void);
void PhantomMissile_State_Reattach(void);
void PhantomMissile_State_Destroyed(void);

#endif //! OBJ_PHANTOMMISSILE_H
