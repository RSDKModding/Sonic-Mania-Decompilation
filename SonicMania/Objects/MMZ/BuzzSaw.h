#ifndef OBJ_BUZZSAW_H
#define OBJ_BUZZSAW_H

#include "Game.h"

typedef enum {
    BUZZSAW_ATTACHED,
    BUZZSAW_STRAY,
} BuzzsawTypes;

// Object Class
struct ObjectBuzzSaw {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    Vector2 unused1;
    int32 unused2;
};

// Entity Class
struct EntityBuzzSaw {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    Vector2 amplitude;
    int32 speed;
    Vector2 startPos;
    Vector2 drawPos;
    Animator animator;
};

// Object Struct
extern ObjectBuzzSaw *BuzzSaw;

// Standard Entity Events
void BuzzSaw_Update(void);
void BuzzSaw_LateUpdate(void);
void BuzzSaw_StaticUpdate(void);
void BuzzSaw_Draw(void);
void BuzzSaw_Create(void *data);
void BuzzSaw_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BuzzSaw_EditorDraw(void);
void BuzzSaw_EditorLoad(void);
#endif
void BuzzSaw_Serialize(void);

// Extra Entity Functions
bool32 BuzzSaw_SfxCheck_SawSus(void);
void BuzzSaw_SfxUpdate_SawSus(int32 sfx);

void BuzzSaw_CheckPlayerCollisions(void);

void BuzzSaw_State_Attached(void);

void BuzzSaw_State_Stray_Waiting(void);
void BuzzSaw_State_Stray_Released(void);
void BuzzSaw_State_FreeMove_Reset(void);

#endif //! OBJ_BUZZSAW_H
