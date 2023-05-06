#ifndef OBJ_SPEAR_H
#define OBJ_SPEAR_H

#include "Game.h"

typedef enum {
    SPEAR_UP,
    SPEAR_RIGHT,
    SPEAR_DOWN,
    SPEAR_LEFT,
} SpearOrientations;

// Object Class
struct ObjectSpear {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxSpear;
};

// Entity Class
struct EntitySpear {
    RSDK_ENTITY
    StateMachine(state); // unused
    uint8 orientation;
    int32 interval;
    int32 intervalOffset;
    int32 duration;
    int32 unused1;
    Vector2 startPos;
    int32 unused2;
    int32 retractPos;
    Hitbox hitbox;
    Animator baseAnimator;
    Animator spearAnimator;
};

// Object Struct
extern ObjectSpear *Spear;

// Standard Entity Events
void Spear_Update(void);
void Spear_LateUpdate(void);
void Spear_StaticUpdate(void);
void Spear_Draw(void);
void Spear_Create(void *data);
void Spear_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Spear_EditorDraw(void);
void Spear_EditorLoad(void);
#endif
void Spear_Serialize(void);

// Extra Entity Functions
void Spear_SetupHitboxes(void);

#endif //! OBJ_SPEAR_H
