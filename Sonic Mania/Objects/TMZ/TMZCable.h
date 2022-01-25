#ifndef OBJ_TMZCABLE_H
#define OBJ_TMZCABLE_H

#include "SonicMania.h"

#define TMZCable_JointCount (8)

// Object Class
struct ObjectTMZCable {
    RSDK_OBJECT
    Hitbox hitbox;
};

// Entity Class
struct EntityTMZCable {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 cableID;
    Vector2 field_64;
    Vector2 drawPos[TMZCable_JointCount];
    bool32 drawFlags[TMZCable_JointCount];
    Vector2 *posPtr;
    Animator animator;
};

// Object Struct
extern ObjectTMZCable *TMZCable;

// Standard Entity Events
void TMZCable_Update(void);
void TMZCable_LateUpdate(void);
void TMZCable_StaticUpdate(void);
void TMZCable_Draw(void);
void TMZCable_Create(void* data);
void TMZCable_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void TMZCable_EditorDraw(void);
void TMZCable_EditorLoad(void);
#endif
void TMZCable_Serialize(void);

// Extra Entity Functions
void TMZCable_Unknown1(void);
void TMZCable_Unknown2(void);
void TMZCable_Unknown3(void);
void TMZCable_Unknown4(void);
void TMZCable_Unknown5(void);
void TMZCable_Unknown6(void);

#endif //!OBJ_TMZCABLE_H
