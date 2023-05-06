#ifndef OBJ_SSZSPIKEBALL_H
#define OBJ_SSZSPIKEBALL_H

#include "Game.h"

typedef enum {
    SSZSPIKEBALL_LAUNCHER_UP,
    SSZSPIKEBALL_LAUNCHER_DOWN,
    SSZSPIKEBALL_LAUNCHER_LEFT,
    SSZSPIKEBALL_LAUNCHER_RIGHT,
    SSZSPIKEBALL_MOVEBALL_UP,
    SSZSPIKEBALL_MOVEBALL_DOWN,
    SSZSPIKEBALL_MOVEBALL_LEFT,
    SSZSPIKEBALL_MOVEBALL_RIGHT,
} SSZSpikeBallTypes;

// Object Class
struct ObjectSSZSpikeBall {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxPon;
    Hitbox hitboxSpikeBall;
    Hitbox hitboxBase[4];
};

// Entity Class
struct EntitySSZSpikeBall {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint16 interval;
    uint16 intervalOffset;
    int32 speed;
    int32 dist;
    uint8 timer;
    Vector2 spikeBallPos;
    Animator baseAnimator;
    Animator spikeBallAnimator;
};

// Object Struct
extern ObjectSSZSpikeBall *SSZSpikeBall;

// Standard Entity Events
void SSZSpikeBall_Update(void);
void SSZSpikeBall_LateUpdate(void);
void SSZSpikeBall_StaticUpdate(void);
void SSZSpikeBall_Draw(void);
void SSZSpikeBall_Create(void *data);
void SSZSpikeBall_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SSZSpikeBall_EditorDraw(void);
void SSZSpikeBall_EditorLoad(void);
#endif
void SSZSpikeBall_Serialize(void);

// Extra Entity Functions
void SSZSpikeBall_State_AwaitInterval(void);
void SSZSpikeBall_State_H(void);
void SSZSpikeBall_State_V(void);
void SSZSpikeBall_State_H_Launch(void);
void SSZSpikeBall_State_V_Launch(void);
void SSZSpikeBall_State_Ball_MoveOut(void);
void SSZSpikeBall_State_Ball_Stopped(void);
void SSZSpikeBall_State_Ball_MoveBack(void);

#endif //! OBJ_SSZSPIKEBALL_H
