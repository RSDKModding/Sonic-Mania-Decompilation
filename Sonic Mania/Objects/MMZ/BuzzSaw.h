#ifndef OBJ_BUZZSAW_H
#define OBJ_BUZZSAW_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    int field_10;
    int field_14;
    int field_18;
} ObjectBuzzSaw;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int type;
    Vector2 amplitude;
    int speed;
    Vector2 startPos;
    Vector2 drawPos;
    Animator animator;
} EntityBuzzSaw;

// Object Struct
extern ObjectBuzzSaw *BuzzSaw;

// Standard Entity Events
void BuzzSaw_Update(void);
void BuzzSaw_LateUpdate(void);
void BuzzSaw_StaticUpdate(void);
void BuzzSaw_Draw(void);
void BuzzSaw_Create(void* data);
void BuzzSaw_StageLoad(void);
void BuzzSaw_EditorDraw(void);
void BuzzSaw_EditorLoad(void);
void BuzzSaw_Serialize(void);

// Extra Entity Functions
bool32 BuzzSaw_CheckCB(void);
void BuzzSaw_UpdateCB(int sfx);

void BuzzSaw_CheckPlayerCollisions(void);

void BuzzSaw_State_OnArm(void);

void BuzzSaw_State_FreeMove_Waiting(void);
void BuzzSaw_State_FreeMove_Released(void);
void BuzzSaw_State_FreeMove_Reset(void);

#endif //!OBJ_BUZZSAW_H
