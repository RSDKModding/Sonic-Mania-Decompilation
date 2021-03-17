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
    void (*state)();
    int field_5C;
    int timer;
    Vector2 startPos;
    byte startDir;
    bool32 wasTurning;
    EntityAnimationData data;
} EntityMotobug;

// Object Struct
extern ObjectMotobug *Motobug;

// Standard Entity Events
void Motobug_Update();
void Motobug_LateUpdate();
void Motobug_StaticUpdate();
void Motobug_Draw();
void Motobug_Create(void* data);
void Motobug_StageLoad();
void Motobug_EditorDraw();
void Motobug_EditorLoad();
void Motobug_Serialize();

// Extra Entity Functions
void Motobug_DebugDraw();
void Motobug_DebugSpawn();
void Motobug_CheckOnScreen();
void Motobug_CheckPlayerCollisions();
//States
void Motobug_State_Fall();
void Motobug_State_HandleMove();
void Motobug_State_Move2();
void Motobug_State_Move();
void Motobug_State_Smoke();
void Motobug_State_Turn();

#endif //!OBJ_MOTOBUG_H
