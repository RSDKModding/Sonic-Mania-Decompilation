#ifndef OBJ_BALL_H
#define OBJ_BALL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
    uint16 sfxSplash;
} ObjectBall;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    Entity *playerPtr;
    Vector2 startPos;
    Animator animator;
} EntityBall;

// Object Entity
extern ObjectBall *Ball;

// Standard Entity Events
void Ball_Update(void);
void Ball_LateUpdate(void);
void Ball_StaticUpdate(void);
void Ball_Draw(void);
void Ball_Create(void* data);
void Ball_StageLoad(void);
void Ball_EditorDraw(void);
void Ball_EditorLoad(void);
void Ball_Serialize(void);

// Extra Entity Functions
void Ball_DebugSpawn(void);
void Ball_DebugDraw(void);

void Ball_HandleInteractions(void);
void Ball_CheckOnScreen(void);
void Ball_SpawnChildren(void);

void Ball_State_Setup(void);
void Ball_State_Unknown1(void);
void Ball_State_Unknown2(void);
void Ball_State_Unknown3(void);

void Ball_State2_Unknown1(void);
void Ball_State3_Unknown1(void);
void Ball_State4_Unknown1(void);

#endif //!OBJ_BALL_H
