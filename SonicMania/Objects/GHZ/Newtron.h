#ifndef OBJ_NEWTRON_H
#define OBJ_NEWTRON_H

#include "Game.h"

typedef enum {
    NEWTRON_SHOOT,
    NEWTRON_FLY,
    NEWTRON_PROJECTILE,
} NewtronTypes;

// Object Class
struct ObjectNewtron {
    RSDK_OBJECT
    Hitbox hitboxShoot;
    Hitbox hitboxFly; // not sure why isn't this used in this object... GHZ/CheckerBall uses it though
    Hitbox hitboxProjectile;
    Hitbox hitboxRange;
    uint16 aniFrames;
};

// Entity Class
struct EntityNewtron {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    int32 timer;
    Vector2 startPos;
    Animator animator;
    Animator flameAnimator;
};

// Object Struct
extern ObjectNewtron *Newtron;

// Standard Entity Events
void Newtron_Update(void);
void Newtron_LateUpdate(void);
void Newtron_StaticUpdate(void);
void Newtron_Draw(void);
void Newtron_Create(void *data);
void Newtron_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Newtron_EditorDraw(void);
void Newtron_EditorLoad(void);
#endif
void Newtron_Serialize(void);

// Extra Entity Functions
void Newtron_DebugDraw(void);
void Newtron_DebugSpawn(void);

// Helpers
void Newtron_CheckPlayerCollisions(void);
void Newtron_CheckOffScreen(void);
void Newtron_GetTargetDir(void);

// States
void Newtron_State_Init(void);
void Newtron_State_CheckPlayerInRange(void);
void Newtron_State_Appear(void);
void Newtron_State_StartFly(void);
void Newtron_State_Fly(void);

void Newtron_State_Shoot(void);
void Newtron_State_FadeAway(void);

void Newtron_State_Projectile(void);

#endif //! OBJ_NEWTRON_H
