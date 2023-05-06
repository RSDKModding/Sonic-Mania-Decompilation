#ifndef OBJ_STAIRCASE_H
#define OBJ_STAIRCASE_H

#include "Game.h"

#define STAIRCASE_STAIR_COUNT (4)

typedef enum {
    STAIRCASE_DOWN,
    STAIRCASE_UP,
} StaircaseTypes;

// Object Class
struct ObjectStaircase {
    RSDK_OBJECT
    Hitbox blockHitbox;
    uint16 aniFrames;
    uint16 unused;
    uint16 sfxBlockStop;
};

// Entity Class
struct EntityStaircase {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 blockPos[STAIRCASE_STAIR_COUNT];
    int32 timer;
    Animator animator;
    uint8 type;
    bool32 bumpable;
};

// Object Struct
extern ObjectStaircase *Staircase;

// Standard Entity Events
void Staircase_Update(void);
void Staircase_LateUpdate(void);
void Staircase_StaticUpdate(void);
void Staircase_Draw(void);
void Staircase_Create(void *data);
void Staircase_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Staircase_EditorDraw(void);
void Staircase_EditorLoad(void);
#endif
void Staircase_Serialize(void);

// Extra Entity Functions
bool32 Staircase_SfxCheck_HitBlocks(void);

void Staircase_State_Idle(void);
void Staircase_State_Wait(void);
void Staircase_State_MoveBlocks(void);
void Staircase_State_MovedBlocks(void);

void Staircase_Draw_Blocks(void);
void Staircase_Draw_Shake(void);

#endif //! OBJ_STAIRCASE_H
