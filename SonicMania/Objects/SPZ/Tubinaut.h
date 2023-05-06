#ifndef OBJ_TUBINAUT_H
#define OBJ_TUBINAUT_H

#include "Game.h"

#define TUBINAUT_ORB_COUNT (3)

// Object Class
struct ObjectTubinaut {
    RSDK_OBJECT
    Hitbox hitboxFace;
    Hitbox hitboxOrb;
    Hitbox hitboxSpin;
    uint16 aniFrames;
    uint16 sfxPowerdown;
    uint16 sfxRepel;
};

// Entity Class
struct EntityTubinaut {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(orbState);
    Animator bodyAnimator;
    Animator fieldAnimator;
    Vector2 startPos;
    Vector2 orbPositions[TUBINAUT_ORB_COUNT];
    int32 orbAngles[TUBINAUT_ORB_COUNT];
    int16 reAdjustTimers[TUBINAUT_ORB_COUNT];
    bool32 ballsVisible[TUBINAUT_ORB_COUNT]; // hehe
    Animator ballAnimators[TUBINAUT_ORB_COUNT];
    uint16 playerTimers[PLAYER_COUNT];
    int32 attackTimer;
    uint16 activeOrbs;
    uint8 orbCount;
    int32 timer;
    uint16 distanceRemain;
    uint16 distance;
};

// Object Struct
extern ObjectTubinaut *Tubinaut;

// Standard Entity Events
void Tubinaut_Update(void);
void Tubinaut_LateUpdate(void);
void Tubinaut_StaticUpdate(void);
void Tubinaut_Draw(void);
void Tubinaut_Create(void *data);
void Tubinaut_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Tubinaut_EditorDraw(void);
void Tubinaut_EditorLoad(void);
#endif
void Tubinaut_Serialize(void);

// Extra Entity Functions
void Tubinaut_DebugSpawn(void);
void Tubinaut_DebugDraw(void);

void Tubinaut_CheckPlayerCollisions(void);
bool32 Tubinaut_CheckAttacking(EntityPlayer *player);
void Tubinaut_OrbHit(EntityPlayer *player, int32 orbID);

void Tubinaut_HandleRepel(EntityPlayer *player, int32 playerID);
void Tubinaut_HandleOrbs(void);
void Tubinaut_CheckOffScreen(void);

void Tubinaut_State_Init(void);
void Tubinaut_State_Move(void);

void Tubinaut_Orb_Relax(void);
void Tubinaut_Orb_PrepareAttack(void);
void Tubinaut_Orb_Attack(void);
void Tubinaut_Orb_Cooldown(void);

void Tubinaut_Orb_BodyDeath(void);

#endif //! OBJ_TUBINAUT_H
