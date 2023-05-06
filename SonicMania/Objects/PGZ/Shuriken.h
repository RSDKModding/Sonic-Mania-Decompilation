#ifndef OBJ_SHURIKEN_H
#define OBJ_SHURIKEN_H

#include "Game.h"

// Object Class
struct ObjectShuriken {
    RSDK_OBJECT
    Hitbox hitboxShooter;
    Hitbox hitboxRange;
    Hitbox hitboxShuriken;
    Hitbox hitboxPlatform;
    int32 timerGroups[255];
    uint16 aniFrames;
    uint16 sfxArrowLaunch;
    uint16 sfxArrowHit;
};

// Entity Class
struct EntityShuriken {
    RSDK_ENTITY
    StateMachine(state);
    uint16 timer;
    uint16 dropTimer;
    uint8 activePlayers;
    Animator animator;
    uint16 delay;
    bool32 timerMode;
    uint16 interval;
    uint16 intervalOffset;
    uint8 timerGroup;
};

// Object Struct
extern ObjectShuriken *Shuriken;

// Standard Entity Events
void Shuriken_Update(void);
void Shuriken_LateUpdate(void);
void Shuriken_StaticUpdate(void);
void Shuriken_Draw(void);
void Shuriken_Create(void *data);
void Shuriken_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Shuriken_EditorDraw(void);
void Shuriken_EditorLoad(void);
#endif
void Shuriken_Serialize(void);

// Extra Entity Functions
void Shuriken_CheckPlayerCollisions(void);
void Shuriken_HandleSolidCollisions(void);

// Shooter States
void Shuriken_State_Init(void);
void Shuriken_State_AwaitActivate(void);
void Shuriken_State_CheckPlayerInRange(void);
void Shuriken_State_ShootDelay(void);
void Shuriken_State_FireShuriken(void);
void Shuriken_State_ShurikenFired(void);
void Shuriken_State_Deactivate(void);

// Arrow States
void Shuriken_State_InitShuriken(void);
void Shuriken_State_ShurikenThrown(void);
void Shuriken_State_ShurikenStuck(void);
void Shuriken_State_ShurikenFall(void);
#if MANIA_USE_PLUS
void Shuriken_State_ShurikenDebris(void);
#endif

#endif //! OBJ_SHURIKEN_H
