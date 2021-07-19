#ifndef OBJ_MOTOBUG_H
#define OBJ_MOTOBUG_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort spriteIndex;
} ObjectMotobug;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int turnTimer;
    int timer;
    Vector2 startPos;
    byte startDir;
    bool32 wasTurning;
    Animator animator;
} EntityMotobug;

// Object Struct
extern ObjectMotobug *Motobug;

// Standard Entity Events
void Motobug_Update(void);
void Motobug_LateUpdate(void);
void Motobug_StaticUpdate(void);
void Motobug_Draw(void);
void Motobug_Create(void* data);
void Motobug_StageLoad(void);
void Motobug_EditorDraw(void);
void Motobug_EditorLoad(void);
void Motobug_Serialize(void);

// Extra Entity Functions
void Motobug_DebugDraw(void);
void Motobug_DebugSpawn(void);
void Motobug_CheckOnScreen(void);
void Motobug_CheckPlayerCollisions(void);
//States
void Motobug_State_Fall(void);
void Motobug_State_HandleMove(void);
void Motobug_State_Move2(void);
void Motobug_State_Move(void);
void Motobug_State_Smoke(void);
void Motobug_State_Turn(void);

#endif //!OBJ_MOTOBUG_H
