#ifndef OBJ_TMZCABLE_H
#define OBJ_TMZCABLE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
} ObjectTMZCable;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    int cableID;
    Vector2 field_64;
    Vector2 drawPos[8];
    bool32 drawFlags[8];
    Vector2 *posPtr;
    Animator animator;
} EntityTMZCable;

// Object Struct
extern ObjectTMZCable *TMZCable;

// Standard Entity Events
void TMZCable_Update(void);
void TMZCable_LateUpdate(void);
void TMZCable_StaticUpdate(void);
void TMZCable_Draw(void);
void TMZCable_Create(void* data);
void TMZCable_StageLoad(void);
void TMZCable_EditorDraw(void);
void TMZCable_EditorLoad(void);
void TMZCable_Serialize(void);

// Extra Entity Functions
void TMZCable_Unknown1(void);
void TMZCable_Unknown2(void);
void TMZCable_Unknown3(void);
void TMZCable_Unknown4(void);
void TMZCable_Unknown5(void);
void TMZCable_Unknown6(void);

#endif //!OBJ_TMZCABLE_H
