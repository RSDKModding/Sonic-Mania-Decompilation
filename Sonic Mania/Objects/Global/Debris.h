#ifndef OBJ_DEBRIS_H
#define OBJ_DEBRIS_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
} ObjectDebris;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    int gravity;
    int rotSpeed;
    Vector2 scaleInc;
    int cooldown;
    Animator animator;
} EntityDebris;

// Object Struct
extern ObjectDebris *Debris;

// Standard Entity Events
void Debris_Update(void);
void Debris_LateUpdate(void);
void Debris_StaticUpdate(void);
void Debris_Draw(void);
void Debris_Create(void* data);
void Debris_StageLoad(void);
void Debris_EditorDraw(void);
void Debris_EditorLoad(void);
void Debris_Serialize(void);

// Extra Entity Functions
void Debris_State_Fall(void);
void Debris_State_LightningSpark(void);
void Debris_State_FallAndFlicker(void);

void Debris_FallFlickerAnimSetup(int spriteIndex, int *a2, int animationID);
void Debris_FallFlickerSetup(int spriteIndex, int *a2);

#endif //!OBJ_DEBRIS_H
