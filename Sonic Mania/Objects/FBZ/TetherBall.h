#ifndef OBJ_TETHERBALL_H
#define OBJ_TETHERBALL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Animator animator;
    uint16 aniFrames;
    Hitbox hitbox;
    uint16 sfxWhack;
} ObjectTetherBall;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint8 chainCount;
    int angleStart;
    int angleEnd;
    int timer;
    int size;
    int field_70;
    Vector2 posUnknown;
    EntityPlayer *playerPtr;
} EntityTetherBall;

// Object Struct
extern ObjectTetherBall *TetherBall;

// Standard Entity Events
void TetherBall_Update(void);
void TetherBall_LateUpdate(void);
void TetherBall_StaticUpdate(void);
void TetherBall_Draw(void);
void TetherBall_Create(void* data);
void TetherBall_StageLoad(void);
void TetherBall_EditorDraw(void);
void TetherBall_EditorLoad(void);
void TetherBall_Serialize(void);

// Extra Entity Functions
void TetherBall_Unknown1(void);
void TetherBall_Unknown2(void);
void TetherBall_Unknown3(void);


#endif //!OBJ_TETHERBALL_H
