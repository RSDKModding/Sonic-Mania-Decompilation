#ifndef OBJ_DIVEEGGMAN_H
#define OBJ_DIVEEGGMAN_H

#include "Game.h"

typedef enum {
    DIVEEGGMAN_EGGMAN,
    DIVEEGGMAN_BOMB,
} DiveEggmanTypes;

// Object Class
struct ObjectDiveEggman {
    RSDK_OBJECT
    Hitbox unusedHitbox1;
    Hitbox hitboxBomb;
    Hitbox hitboxEggman;
    EntityScrewMobile *screwMobile;
    uint16 diveFrames;
    uint16 aniFrames;
    uint16 sfxBigFan;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxRockemSockem;
};

// Entity Class
struct EntityDiveEggman {
    RSDK_ENTITY
    StateMachine(state);
    int32 startY;
    int32 timer;
    int32 invincibilityTimer;
    uint8 health;
    Animator animator;
};

// Object Struct
extern ObjectDiveEggman *DiveEggman;

// Standard Entity Events
void DiveEggman_Update(void);
void DiveEggman_LateUpdate(void);
void DiveEggman_StaticUpdate(void);
void DiveEggman_Draw(void);
void DiveEggman_Create(void *data);
void DiveEggman_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void DiveEggman_EditorDraw(void);
void DiveEggman_EditorLoad(void);
#endif
void DiveEggman_Serialize(void);

// Extra Entity Functions
void DiveEggman_Hit(void);
void DiveEggman_Explode(void);

void DiveEggman_StateEggman_AwaitPlayer(void);
void DiveEggman_StateEggman_Swimming(void);
void DiveEggman_StateEggman_InWhirlpool(void);
void DiveEggman_StateEggman_WhirlpoolRise(void);
void DiveEggman_StateEggman_Falling(void);
void DiveEggman_StateEggman_PlaceBomb(void);
void DiveEggman_StateEggman_Destroyed(void);
void DiveEggman_StateEggman_Finish(void);

bool32 DiveEggman_CheckNoBombExplode(void);

void DiveEggman_StateBomb_Idle(void);
void DiveEggman_StateBomb_InWhirlpool(void);
void DiveEggman_StateBomb_WhirlpoolRise(void);
void DiveEggman_StateBomb_Falling(void);

#endif //! OBJ_DIVEEGGMAN_H
