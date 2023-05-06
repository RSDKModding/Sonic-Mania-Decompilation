#ifndef OBJ_SPLATS_H
#define OBJ_SPLATS_H

#include "Game.h"

typedef enum {
    // GHZ
    SPLATS_BOUNCEAROUND = 0,

    // PGZ
    SPLATS_SPAWNER = 0,
    SPLATS_INKSPLATS,
    SPLATS_SPLAT,
} SplatsTypes;

// Object Class
struct ObjectSplats {
    RSDK_OBJECT
    StateMachine(initialState);
    Hitbox hitboxBadnikGHZ;
    Hitbox hitboxJar;
    Hitbox hitboxBadnikPGZ;
    uint16 aniFrames;
    uint16 sfxSplatsSpawn;
    uint16 sfxSplatsLand;
};

// Entity Class
struct EntitySplats {
    RSDK_ENTITY
    StateMachine(state);
    uint8 bounceCount;
    uint8 activeCount;
    uint8 numActive;
    uint16 minDelay;
    int32 delay;
    bool32 isOnScreen;
    EntitySplats *parent;
    Vector2 startPos;
    int32 startDir;
    Animator mainAnimator;
    Animator splashAnimator;
};

// Object Struct
extern ObjectSplats *Splats;

// Standard Entity Events
void Splats_Update(void);
void Splats_LateUpdate(void);
void Splats_StaticUpdate(void);
void Splats_Draw(void);
void Splats_Create(void *data);
void Splats_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Splats_EditorDraw(void);
void Splats_EditorLoad(void);
#endif
void Splats_Serialize(void);

// Extra Entity Functions
void Splats_DebugDraw(void);
void Splats_DebugSpawn(void);

void Splats_CheckPlayerCollisions(void);
void Splats_CheckOffScreen(void);

void Splats_State_Init(void);
void Splats_State_BounceAround(void);
void Splats_State_SetupInkJar(void);
void Splats_State_InkJarSpawner(void);
void Splats_State_JumpOutOfJar(void);
void Splats_State_HandleBouncing(void);
void Splats_State_HandleLanding(void);
void Splats_State_NoMoreJumps(void);
void Splats_State_InkSplat(void);

#endif //! OBJ_SPLATS_H
