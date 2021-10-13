#ifndef OBJ_STAIRCASE_H
#define OBJ_STAIRCASE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox blockHitbox;
    uint16 animID;
    uint8 gapE[2]; //?
    uint16 sfxID;
} ObjectStaircase;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(drawState);
    Vector2 blocks[4];
    int32 timer;
    Animator animator;
    uint8 type;
    bool32 bumpable;
} EntityStaircase;

// Object Struct
extern ObjectStaircase *Staircase;

// Standard Entity Events
void Staircase_Update(void);
void Staircase_LateUpdate(void);
void Staircase_StaticUpdate(void);
void Staircase_Draw(void);
void Staircase_Create(void *data);
void Staircase_StageLoad(void);
void Staircase_EditorDraw(void);
void Staircase_EditorLoad(void);
void Staircase_Serialize(void);

// Extra Entity Functions
bool32 Staircase_SFXCallback(void);

void Staircase_MainState(void);
void Staircase_Wait(void);
void Staircase_MoveBlocks(void);
void Staircase_BasicCollision(void);

void Staircase_DrawBlocks(void);
void Staircase_DrawShake(void);

#endif //! OBJ_STAIRCASE_H
