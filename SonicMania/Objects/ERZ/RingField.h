#ifndef OBJ_RINGFIELD_H
#define OBJ_RINGFIELD_H

#include "Game.h"

// Object Class
struct ObjectRingField {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityRingField {
    RSDK_ENTITY
    StateMachine(state); // unused
    Vector2 size;
    int32 frequency;
    int32 fluctuation;
    bool32 running;
    int32 unused1;
    Vector2 startPos;
    int32 timer;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectRingField *RingField;

// Standard Entity Events
void RingField_Update(void);
void RingField_LateUpdate(void);
void RingField_StaticUpdate(void);
void RingField_Draw(void);
void RingField_Create(void *data);
void RingField_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void RingField_EditorDraw(void);
void RingField_EditorLoad(void);
#endif
void RingField_Serialize(void);

// Extra Entity Functions
void RingField_GetRingSpawnPos(Vector2 *pos);

#endif //! OBJ_RINGFIELD_H
