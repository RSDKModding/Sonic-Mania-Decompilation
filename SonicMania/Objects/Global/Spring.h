#ifndef OBJ_SPRING_H
#define OBJ_SPRING_H

#include "Game.h"

typedef enum {
    SPRING_VERT_YELLOW,
    SPRING_VERT_RED,
    SPRING_HORIZ_YELLOW,
    SPRING_HORIZ_RED,
    SPRING_DIAG_YELLOW,
    SPRING_DIAG_RED,
    SPRING_TUBESPRING = 0xFF,
} SpringTypes;

// Object Class
struct ObjectSpring {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxSpring;
};

// Entity Class
struct EntitySpring {
    RSDK_ENTITY
    StateMachine(state);
    SpringTypes type;
    FlipFlags flipFlag;
    uint8 planeFilter;
    int32 sfxTimer;
    Animator animator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectSpring *Spring;

// Standard Entity Events
void Spring_Update(void);
void Spring_LateUpdate(void);
void Spring_StaticUpdate(void);
void Spring_Draw(void);
void Spring_Create(void *data);
void Spring_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Spring_EditorDraw(void);
void Spring_EditorLoad(void);
#endif
void Spring_Serialize(void);

// Extra Entity Functions
void Spring_State_Vertical(void);
void Spring_State_Horizontal(void);
void Spring_State_Diagonal(void);

#endif //! OBJ_SPRING_H
