#ifndef OBJ_STAIRCASE_H
#define OBJ_STAIRCASE_H

#include "SonicMania.h"

#define Starcase_StairCount (4)

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox blockHitbox;
    uint16 animID;
    uint16 unused;
    uint16 sfxID;
} ObjectStaircase;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 blocks[Starcase_StairCount];
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
#if RETRO_INCLUDE_EDITOR
void Staircase_EditorDraw(void);
void Staircase_EditorLoad(void);
#endif
void Staircase_Serialize(void);

// Extra Entity Functions
bool32 Staircase_CheckCB(void);

void Staircase_State_Idle(void);
void Staircase_State_Wait(void);
void Staircase_State_MoveBlocks(void);
void Staircase_State_MovedBlocks(void);

void Staircase_Draw_Blocks(void);
void Staircase_Draw_Shake(void);

#endif //! OBJ_STAIRCASE_H
