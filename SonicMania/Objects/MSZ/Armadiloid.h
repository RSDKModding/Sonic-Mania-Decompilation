#ifndef OBJ_ARMADILOID_H
#define OBJ_ARMADILOID_H

#include "Game.h"

typedef enum { ARMADILOID_SHOOTER, ARMADILOID_RIDER } ArmadiloidTypes;

// Object Class
struct ObjectArmadiloid {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxShot;
};

// Entity Class
struct EntityArmadiloid {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 timer;
    EntityArmadiloid *child;
    EntityArmadiloid *parent;
    Vector2 startPos;
    Hitbox hitbox;
    Animator bodyAnimator;
    Animator headAnimator;
    Animator boosterAnimator;
};

// Object Struct
extern ObjectArmadiloid *Armadiloid;

// Standard Entity Events
void Armadiloid_Update(void);
void Armadiloid_LateUpdate(void);
void Armadiloid_StaticUpdate(void);
void Armadiloid_Draw(void);
void Armadiloid_Create(void *data);
void Armadiloid_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Armadiloid_EditorDraw(void);
void Armadiloid_EditorLoad(void);
#endif
void Armadiloid_Serialize(void);

// Extra Entity Functions
void Armadiloid_DebugSpawn(void);
void Armadiloid_DebugDraw(void);

void Armadiloid_State_Init(void);

void Armadiloid_State_PlatformFlying(void);
void Armadiloid_PlatformShootDelay(void);
void Armadiloid_State_PlatformShoot(void);

void Armadiloid_State_Rider(void);

#endif //! OBJ_ARMADILOID_H
