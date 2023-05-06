#ifndef OBJ_CHOPPER_H
#define OBJ_CHOPPER_H

#include "Game.h"

typedef enum { CHOPPER_JUMP, CHOPPER_SWIM } ChopperTypes;

// Object Class
struct ObjectChopper {
    RSDK_OBJECT
    Hitbox hitboxJump;
    Hitbox hitboxSwim;
    Hitbox hitboxRange;
    Hitbox hitboxWater;
    uint16 aniFrames;
};

// Entity Class
struct EntityChopper {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint16 timer;
    bool32 charge;
    Vector2 startPos;
    uint8 startDir;
    Animator animator;
};

// Object Struct
extern ObjectChopper *Chopper;

// Standard Entity Events
void Chopper_Update(void);
void Chopper_LateUpdate(void);
void Chopper_StaticUpdate(void);
void Chopper_Draw(void);
void Chopper_Create(void *data);
void Chopper_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Chopper_EditorDraw(void);
void Chopper_EditorLoad(void);
#endif
void Chopper_Serialize(void);

// Extra Entity Functions
void Chopper_DebugDraw(void);
void Chopper_DebugSpawn(void);

void Chopper_CheckOffScreen(void);
void Chopper_CheckPlayerCollisions_Jump(void);
void Chopper_CheckPlayerCollisions_Swim(void);

// States
void Chopper_State_Init(void);
void Chopper_State_Jump(void);
void Chopper_State_Swim(void);
void Chopper_State_ChargeDelay(void);
void Chopper_State_Charge(void);

#endif //! OBJ_CHOPPER_H
