#ifndef OBJ_FLAMESPRING_H
#define OBJ_FLAMESPRING_H

#include "Game.h"

typedef enum {
    FLAMESPRING_BOTH,
    FLAMESPRING_RIGHT,
    FLAMESPRING_LEFT,
    FLAMESPRING_BOTH_NOSPRING,
    FLAMESPRING_RIGHT_NOSPRING,
    FLAMESPRING_LEFT_NOSPRING,
} FlameSpringTypes;

typedef enum {
    FLAMESPRING_FORCE_WEAK,
    FLAMESPRING_FORCE_STRONG,
} FlameSpringForceTypes;

typedef enum {
    FLAMESPRING_PANI_DIAGONAL,
    FLAMESPRING_PANI_TWIRL,
} FlameSpringPAniTypes;

// Object Class
struct ObjectFlameSpring {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxSpring;
    Hitbox hitboxFlame;
    uint16 sfxSpring;
    uint16 sfxFlame;
};

// Entity Class
struct EntityFlameSpring {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 force;
    uint8 playerAni;
    uint8 offset;
    Vector2 flamePosL;
    Vector2 flamePosR;
    bool32 spittingFire;
    uint8 timer;
    Animator mainAnimator;
    Animator nozzleAnimator;
};

// Object Struct
extern ObjectFlameSpring *FlameSpring;

// Standard Entity Events
void FlameSpring_Update(void);
void FlameSpring_LateUpdate(void);
void FlameSpring_StaticUpdate(void);
void FlameSpring_Draw(void);
void FlameSpring_Create(void *data);
void FlameSpring_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FlameSpring_EditorDraw(void);
void FlameSpring_EditorLoad(void);
#endif
void FlameSpring_Serialize(void);

// Extra Entity Functions
void FlameSpring_State_Spring(void);
void FlameSpring_State_Flame(void);

void FlameSpring_Draw_Spring(void);
void FlameSpring_Draw_Flame(void);

#endif //! OBJ_FLAMESPRING_H
