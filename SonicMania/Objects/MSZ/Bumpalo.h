#ifndef OBJ_BUMPALO_H
#define OBJ_BUMPALO_H

#include "Game.h"

// Object Class
struct ObjectBumpalo {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxBumper;
    Hitbox hitboxRange;
    Hitbox hitboxCharge;
    Hitbox hitboxUnused;
    uint16 aniFrames;
    uint16 sfxBumper;
    uint16 sfxHuff;
    uint16 sfxClack;
    uint16 sfxImpact;
};

// Entity Class
struct EntityBumpalo {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    int32 timer;
    bool32 ignoreCliffs;
    bool32 wallCollided;
    Animator badnikAnimator;
    Animator huffAnimator;
    Animator dustAnimator;
};

// Object Struct
extern ObjectBumpalo *Bumpalo;

// Standard Entity Events
void Bumpalo_Update(void);
void Bumpalo_LateUpdate(void);
void Bumpalo_StaticUpdate(void);
void Bumpalo_Draw(void);
void Bumpalo_Create(void *data);
void Bumpalo_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Bumpalo_EditorDraw(void);
void Bumpalo_EditorLoad(void);
#endif
void Bumpalo_Serialize(void);

// Extra Entity Functions
void Bumpalo_DebugSpawn(void);
void Bumpalo_DebugDraw(void);

void Bumpalo_CheckOffScreen(void);
void Bumpalo_BumpPlayer(EntityPlayer *player);
void Bumpalo_CheckPlayerCollisions(void);
void Bumpalo_HandlePlatformCollisions(EntityPlatform *platform);
void Bumpalo_HandleObjectCollisions(void);

void Bumpalo_State_Init(void);
void Bumpalo_State_Moving(void);
void Bumpalo_State_Idle(void);
void Bumpalo_State_Charging(void);
void Bumpalo_State_Turning(void);
void Bumpalo_State_Bumped(void);
void Bumpalo_State_Falling(void);

#endif //! OBJ_BUMPALO_H
