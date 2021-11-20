#ifndef OBJ_EGGMAN_H
#define OBJ_EGGMAN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectEggman;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateStore);
    int32 timer;
    uint16 animID;
    Entity* parent;
    Vector2 offset;
    Hitbox hitbox;
    Animator animator;
} EntityEggman;

// Object Struct
extern ObjectEggman *Eggman;

// Standard Entity Events
void Eggman_Update(void);
void Eggman_LateUpdate(void);
void Eggman_StaticUpdate(void);
void Eggman_Draw(void);
void Eggman_Create(void* data);
void Eggman_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Eggman_EditorDraw(void);
void Eggman_EditorLoad(void);
#endif
void Eggman_Serialize(void);

// Extra Entity Functions
void Eggman_State_ProcessAnimation(void);
void Eggman_State_ProcessThenSet(void);
void Eggman_State_ProcessUntilEnd(void);
void Eggman_State_ProcessAirThenSet(void);
void Eggman_State_FallUntilTimerReset(void);
void Eggman_State_FallAndCollide(void);
void Eggman_State_WalkOffScreen(void);

#endif //!OBJ_EGGMAN_H
