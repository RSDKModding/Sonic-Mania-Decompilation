#ifndef OBJ_PISTON_H
#define OBJ_PISTON_H

#include "Game.h"

typedef enum {
    PISTON_MOVE_VERTICAL,
    PISTON_UP,
    PISTON_MOVE_DOWN,
    PISTON_MOVE_DOWN_REVERSE,
    PISTON_MOVE_RIGHT,
    PISTON_MOVE_LEFT,
    PISTON_MOVE_HORIZONTAL,
} PistonTypes;

typedef enum {
    PISTON_SIZE_1,
    PISTON_SIZE_2,
    PISTON_SIZE_3,
} PistonSizes;

// Object Class
struct ObjectPiston {
    RSDK_OBJECT
    uint16 sfxLand;
    uint16 sfxLaunch;
};

// Entity Class
struct EntityPiston {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    int32 type;
    Vector2 amplitude;
    int32 speed;
    bool32 reverse;
    int8 size;
    uint8 collision;
    Vector2 tileOrigin;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int32 stood;
    int32 timer;
    int32 stoodAngle;
    uint8 stoodPlayers;
    uint8 pushPlayersL;
    uint8 pushPlayersR;
    Hitbox hitbox;
    Animator animator;
    int32 childCount;

    uint16 interval;
    uint16 intervalOffset;
    int32 distance;
    int32 pistonType;
};

// Object Struct
extern ObjectPiston *Piston;

// Standard Entity Events
void Piston_Update(void);
void Piston_LateUpdate(void);
void Piston_StaticUpdate(void);
void Piston_Draw(void);
void Piston_Create(void *data);
void Piston_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Piston_EditorDraw(void);
void Piston_EditorLoad(void);
#endif
void Piston_Serialize(void);

// Extra Entity Functions

void Piston_Collide_Solid(void);

void Piston_State_WaitForInterval(void);

void Piston_StateMove_Down(void);
void Piston_StateMove_Down_Reverse(void);
void Piston_StateMove_Vertical(void);
void Piston_StateMove_Vertical_Reverse(void);
void Piston_StateMove_Up(void);
void Piston_StateMove_Up_Reverse(void);
void Piston_StateMove_Right(void);
void Piston_StateMove_Left(void);
void Piston_StateMove_Horizontal(void);
void Piston_StateMove_Horizontal_Reverse(void);

void Piston_StateActive_WaitForStood(void);
void Piston_StateActive_PreparingLaunch(void);
void Piston_StateActive_LaunchPlayers(void);
void Piston_StateActive_ReturnToStartPos(void);

#endif //! OBJ_PISTON_H
