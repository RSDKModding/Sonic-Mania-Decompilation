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
    int32 timer;
    int32 gravity;
    int32 rotSpeed;
    Vector2 scaleInc;
    int32 delay;
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
#if RETRO_INCLUDE_EDITOR
void Debris_EditorDraw(void);
void Debris_EditorLoad(void);
#endif
void Debris_Serialize(void);

// Extra Entity Functions
void Debris_State_Fall(void);
void Debris_State_LightningSpark(void);
void Debris_State_FallAndFlicker(void);

void Debris_FallFlickerAnimSetup(int32 spriteIndex, int32 *a2, int32 animationID);
void Debris_FallFlickerSetup(int32 spriteIndex, int32 *a2);

#endif //!OBJ_DEBRIS_H
