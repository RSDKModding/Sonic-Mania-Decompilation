#ifndef OBJ_TETHERBALL_H
#define OBJ_TETHERBALL_H

#include "Game.h"

typedef enum { TETHERBALL_DOWN, TETHERBALL_UP, TETHERBALL_LEFT, TETHERBALL_RIGHT } TetherBallTypes;

// Object Class
struct ObjectTetherBall {
    RSDK_OBJECT
    Animator animator;
    uint16 aniFrames;
    Hitbox hitbox;
    uint16 sfxWhack;
};

// Entity Class
struct EntityTetherBall {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint8 chainCount;
    int32 angleStart;
    int32 angleEnd;
    int32 timer;
    int32 size;
    int32 swingSpeed;
    Vector2 ballPos;
    EntityPlayer *player;
};

// Object Struct
extern ObjectTetherBall *TetherBall;

// Standard Entity Events
void TetherBall_Update(void);
void TetherBall_LateUpdate(void);
void TetherBall_StaticUpdate(void);
void TetherBall_Draw(void);
void TetherBall_Create(void *data);
void TetherBall_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TetherBall_EditorDraw(void);
void TetherBall_EditorLoad(void);
#endif
void TetherBall_Serialize(void);

// Extra Entity Functions
void TetherBall_State_CheckPlayerAttach(void);
void TetherBall_State_SwingBall(void);
void TetherBall_State_FinishedSwing(void);

#endif //! OBJ_TETHERBALL_H
