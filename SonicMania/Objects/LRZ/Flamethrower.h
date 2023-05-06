#ifndef OBJ_FLAMETHROWER_H
#define OBJ_FLAMETHROWER_H

#include "Game.h"

typedef enum {
    FLAMETHROWER_ORIENTATION_RIGHT,
    FLAMETHROWER_ORIENTATION_DOWN,
    FLAMETHROWER_ORIENTATION_LEFT,
    FLAMETHROWER_ORIENTATION_UP
} FlamethrowerOrientations;

typedef enum {
    FLAMETHROWER_MODE_EMITTER,
    FLAMETHROWER_MODE_SPRINKLER,
} FlamethrowerModes;

// Object Class
struct ObjectFlamethrower {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxFlame;
    bool32 playingFlameSfx;
    Hitbox hitboxMouthH;
    Hitbox hitboxMouthV;
    Hitbox hitboxSmallFireball;
    Hitbox hitboxMediumFireball;
    Hitbox hitboxLargeFireball;
};

// Entity Class
struct EntityFlamethrower {
    RSDK_ENTITY
    StateMachine(state);
    uint8 orientation;
    uint8 mode;
    int32 maxDist;
    uint16 spread;
    uint16 interval;
    uint16 intervalOffset;
    uint16 duration;
    uint16 timer;
    uint16 flameAngle;
    int32 lastYVelocity;
    int32 currentDist;
    EntityFlamethrower *parent;
    Vector2 origin;
    Vector2 lastPos;
    Animator animator;
};

// Object Struct
extern ObjectFlamethrower *Flamethrower;

// Standard Entity Events
void Flamethrower_Update(void);
void Flamethrower_LateUpdate(void);
void Flamethrower_StaticUpdate(void);
void Flamethrower_Draw(void);
void Flamethrower_Create(void *data);
void Flamethrower_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Flamethrower_EditorDraw(void);
void Flamethrower_EditorLoad(void);
#endif
void Flamethrower_Serialize(void);

// Extra Entity Functions
void Flamethrower_SetupOrientation(uint8 orientation);
Hitbox Flamethrower_GetHitbox(void);
void Flamethrower_CheckOffScreen(void);
void Flamethrower_HandleAnimations(void);
void Flamethrower_HandleAngles(void);
void Flamethrower_HandleTileCollisions(void);
void Flamethrower_CheckOutOfBounds(uint8 orientation);

void Flamethrower_CheckFlameCollisions(void);
void Flamethrower_CheckMouthCollisions(void);

void Flamethrower_State_Init(void);
void Flamethrower_State_AwaitInterval(void);
void Flamethrower_State_EmittingFlames(void);

void Flamethrower_State_SetupFireball(void);
void Flamethrower_State_Fireball(void);

#endif //! OBJ_FLAMETHROWER_H
