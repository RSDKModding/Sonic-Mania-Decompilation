#ifndef OBJ_CHOPPER_H
#define OBJ_CHOPPER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    ushort spriteIndex;
} ObjectChopper;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    byte type;
    ushort timer;
    bool32 charge;
    Vector2 startPos;
    byte startDir;
    AnimationData data;
} EntityChopper;

// Object Struct
extern ObjectChopper *Chopper;

// Standard Entity Events
void Chopper_Update();
void Chopper_LateUpdate();
void Chopper_StaticUpdate();
void Chopper_Draw();
void Chopper_Create(void* data);
void Chopper_StageLoad();
void Chopper_EditorDraw();
void Chopper_EditorLoad();
void Chopper_Serialize();

// Extra Entity Functions
void Chopper_DebugDraw();
void Chopper_DebugSpawn();

void Chopper_CheckOnScreen();
void Chopper_Unknown3();
void Chopper_Unknown4();
void Chopper_Unknown5();
void Chopper_Unknown6();
void Chopper_Unknown7();
void Chopper_CheckPlayerCollisions();
void Chopper_CheckPlayerCollisions2();

#endif //!OBJ_CHOPPER_H
