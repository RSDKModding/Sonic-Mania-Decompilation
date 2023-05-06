#ifndef OBJ_SWEEP_H
#define OBJ_SWEEP_H

#include "Game.h"

// Object Class
struct ObjectSweep {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxRange;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
    uint16 sfxPon;
};

// Entity Class
struct EntitySweep {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateStore);
    int32 timer;
    uint8 hasShot;
    Vector2 startPos;
    uint8 startDir;
    EntityWater *water;
    Animator animator;
};

// Object Struct
extern ObjectSweep *Sweep;

// Standard Entity Events
void Sweep_Update(void);
void Sweep_LateUpdate(void);
void Sweep_StaticUpdate(void);
void Sweep_Draw(void);
void Sweep_Create(void *data);
void Sweep_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Sweep_EditorDraw(void);
void Sweep_EditorLoad(void);
#endif
void Sweep_Serialize(void);

// Extra Entity Functions
void Sweep_DebugSpawn(void);
void Sweep_DebugDraw(void);

void Sweep_CheckOffScreen(void);
void Sweep_CheckPlayerCollisions(void);
void Sweep_CheckShoot(void);

void Sweep_State_Init(void);
void Sweep_State_Idle(void);
void Sweep_State_Dash(void);
void Sweep_State_Stop(void);
void Sweep_State_FiredShot(void);
void Sweep_State_Turn(void);
void Sweep_State_Projectile(void);

#endif //! OBJ_SWEEP_H
