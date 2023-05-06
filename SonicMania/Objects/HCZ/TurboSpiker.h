#ifndef OBJ_TURBOSPIKER_H
#define OBJ_TURBOSPIKER_H

#include "Game.h"

typedef enum {
    TURBOSPIKER_VISIBLE,
    TURBOSPIKER_HIDDEN,
} TurboSpikerTypes;

// Object Class
struct ObjectTurboSpiker {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxSpike;
    Hitbox hitboxRange;
    uint16 aniFrames;
    uint16 sfxLaunch;
    uint16 sfxSplash;
};

// Entity Class
struct EntityTurboSpiker {
    RSDK_ENTITY
    StateMachine(state);
    uint32 type;
    int32 timer;
    int32 unused;
    Vector2 startPos;
    uint8 startDir;
    EntityTurboSpiker *spike;
    Animator shellAnimator;
    Animator animator;
    bool32 playedLaunchSfx;
};

// Object Struct
extern ObjectTurboSpiker *TurboSpiker;

// Standard Entity Events
void TurboSpiker_Update(void);
void TurboSpiker_LateUpdate(void);
void TurboSpiker_StaticUpdate(void);
void TurboSpiker_Draw(void);
void TurboSpiker_Create(void *data);
void TurboSpiker_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TurboSpiker_EditorDraw(void);
void TurboSpiker_EditorLoad(void);
#endif
void TurboSpiker_Serialize(void);

void TurboSpiker_DebugSpawn(void);
void TurboSpiker_DebugDraw(void);

// Extra Entity Functions
void TurboSpiker_Hermit_Collide(void);
void TurboSpiker_Hermit_CheckOffScreen(void);
void TurboSpiker_HandleMovement(void);

void TurboSpiker_State_Init(void);
void TurboSpiker_State_Idle(void);
void TurboSpiker_State_Hidden(void);
void TurboSpiker_State_Moving(void);
void TurboSpiker_State_Turning(void);
void TurboSpiker_State_FinishTurning(void);
void TurboSpiker_State_Falling(void);
void TurboSpiker_State_FireShell(void);

void TurboSpiker_HandleSpikeCollisions(void);
void TurboSpiker_State_Spike(void);

void TurboSpiker_State_Ember(void);

#endif //! OBJ_TURBOSPIKER_H
