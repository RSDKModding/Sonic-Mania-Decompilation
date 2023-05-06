#ifndef OBJ_MMZWHEEL_H
#define OBJ_MMZWHEEL_H

#include "Game.h"

typedef enum {
    MMZWHEEL_MOTION_STIFF,
    MMZWHEEL_MOTION_SMOOTH,
} MMZWheelMotionTypes;

// Object Class
struct ObjectMMZWheel {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityMMZWheel {
    RSDK_ENTITY
    int32 amplitude;
    uint8 childCount;
    int32 speed;
    uint8 motionType;
    Vector2 center;
    Vector2 offset;
    int32 unused;
    Animator animator;
};

// Object Struct
extern ObjectMMZWheel *MMZWheel;

// Standard Entity Events
void MMZWheel_Update(void);
void MMZWheel_LateUpdate(void);
void MMZWheel_StaticUpdate(void);
void MMZWheel_Draw(void);
void MMZWheel_Create(void *data);
void MMZWheel_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MMZWheel_EditorDraw(void);
void MMZWheel_EditorLoad(void);
#endif
void MMZWheel_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_MMZWHEEL_H
