#ifndef OBJ_FLAMETHROWER_H
#define OBJ_FLAMETHROWER_H

#include "SonicMania.h"

typedef enum {
    FLAMETHROWER_ORIENTATION_RIGHT,
    FLAMETHROWER_ORIENTATION_DOWN,
    FLAMETHROWER_ORIENTATION_LEFT,
    FLAMETHROWER_ORIENTATION_UP
} FlamethrowerOrientations;

// Object Class
struct ObjectFlamethrower {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxFlame;
    bool32 playingFlameSfx;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    Hitbox hitbox5;
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
    int32 field_70;
    int32 field_74;
    Entity *parent;
    Vector2 origin;
    Vector2 field_84;
    Animator animator;
};

// Object Struct
extern ObjectFlamethrower *Flamethrower;

// Standard Entity Events
void Flamethrower_Update(void);
void Flamethrower_LateUpdate(void);
void Flamethrower_StaticUpdate(void);
void Flamethrower_Draw(void);
void Flamethrower_Create(void* data);
void Flamethrower_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Flamethrower_EditorDraw(void);
void Flamethrower_EditorLoad(void);
#endif
void Flamethrower_Serialize(void);

// Extra Entity Functions
void Flamethrower_SetupOrientation(uint8 orientation);
Hitbox Flamethrower_GetHitbox(void);
void Flamethrower_CheckOnScreen(void);
void Flamethrower_Unknown3(void);
void Flamethrower_HandleAngles(void);
void Flamethrower_HandleTileCollisions(void);
void Flamethrower_Unknown7(uint8 orientation);

void Flamethrower_CheckFlameCollisions(void);
void Flamethrower_CheckBaseCollisions(void);

void Flamethrower_State_Unknown1(void);
void Flamethrower_State_Unknown2(void);
void Flamethrower_State_Unknown3(void);

void Flamethrower_State1_Unknown1(void);
void Flamethrower_State1_Unknown2(void);

#endif //!OBJ_FLAMETHROWER_H
