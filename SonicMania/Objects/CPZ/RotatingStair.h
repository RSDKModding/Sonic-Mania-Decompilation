#ifndef OBJ_ROTATINGSTAIR_H
#define OBJ_ROTATINGSTAIR_H

#include "Game.h"

typedef enum {
    ROTATINGSTAIR_LEFT,
    ROTATINGSTAIR_DOWN,
    ROTATINGSTAIR_RIGHT,
    ROTATINGSTAIR_UP,
    ROTATINGSTAIR_LEFT_INTERVAL,
    ROTATINGSTAIR_DOWN_INTERVAL,
    ROTATINGSTAIR_RIGHT_INTERVAL,
    ROTATINGSTAIR_UP_INTERVAL,
} RotatingStairModes;

// Object Class
struct ObjectRotatingStair {
    RSDK_OBJECT
    uint8 unused; // its set to 0, but its not actually used
};

// Entity Class
struct EntityRotatingStair {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    RotatingStairModes mode;
    Vector2 amplitude;
    int32 speed;
    bool32 hasTension;
    int8 frameID;
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

    uint16 oscOff;
    int32 duration;
    int32 interval;
};

// Object Struct
extern ObjectRotatingStair *RotatingStair;

// Standard Entity Events
void RotatingStair_Update(void);
void RotatingStair_LateUpdate(void);
void RotatingStair_StaticUpdate(void);
void RotatingStair_Draw(void);
void RotatingStair_Create(void *data);
void RotatingStair_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void RotatingStair_EditorDraw(void);
void RotatingStair_EditorLoad(void);
#endif
void RotatingStair_Serialize(void);

// Extra Entity Functions
void RotatingStair_State_Move(void);
void RotatingStair_State_Move_Intervals(void);

#endif //! OBJ_ROTATINGSTAIR_H
