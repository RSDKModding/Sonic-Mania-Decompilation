#ifndef OBJ_MOTOBUG_H
#define OBJ_MOTOBUG_H

#include "Game.h"

// Object Class
struct ObjectMotobug {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    uint16 aniFrames;
};

// Entity Class
struct EntityMotobug {
    RSDK_ENTITY
    StateMachine(state);
    int32 turnTimer;
    int32 timer;
    Vector2 startPos;
    uint8 startDir;
    bool32 wasTurning;
    Animator animator;
};

// Object Struct
extern ObjectMotobug *Motobug;

// Standard Entity Events
void Motobug_Update(void);
void Motobug_LateUpdate(void);
void Motobug_StaticUpdate(void);
void Motobug_Draw(void);
void Motobug_Create(void *data);
void Motobug_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Motobug_EditorDraw(void);
void Motobug_EditorLoad(void);
#endif
void Motobug_Serialize(void);

// Extra Entity Functions
void Motobug_DebugDraw(void);
void Motobug_DebugSpawn(void);

void Motobug_CheckOffScreen(void);
void Motobug_CheckPlayerCollisions(void);

// States
void Motobug_State_Init(void);
void Motobug_State_Move(void);
void Motobug_State_Idle(void);
void Motobug_State_Fall(void);
void Motobug_State_Turn(void);
void Motobug_State_Smoke(void);

#endif //! OBJ_MOTOBUG_H
