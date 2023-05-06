#ifndef OBJ_TMZCABLE_H
#define OBJ_TMZCABLE_H

#include "Game.h"

#define TMZCABLE_JOINT_COUNT (8)

// Object Class
struct ObjectTMZCable {
    RSDK_OBJECT
    Hitbox hitbox;
};

// Entity Class
struct EntityTMZCable {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 cableID;
    Vector2 offset;
    Vector2 jointPos[TMZCABLE_JOINT_COUNT];
    bool32 jointVisible[TMZCABLE_JOINT_COUNT];
    Vector2 *parentPos;
    Animator animator;
};

// Object Struct
extern ObjectTMZCable *TMZCable;

// Standard Entity Events
void TMZCable_Update(void);
void TMZCable_LateUpdate(void);
void TMZCable_StaticUpdate(void);
void TMZCable_Draw(void);
void TMZCable_Create(void *data);
void TMZCable_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TMZCable_EditorDraw(void);
void TMZCable_EditorLoad(void);
#endif
void TMZCable_Serialize(void);

// Extra Entity Functions
void TMZCable_HandleDrawPositions(void);

void TMZCable_State_Idle(void);
void TMZCable_State_Charge(void);
void TMZCable_State_Live(void);
void TMZCable_State_Fade(void);
void TMZCable_State_Destroyed(void);

#endif //! OBJ_TMZCABLE_H
