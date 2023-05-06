#ifndef OBJ_BUZZBOMBER_H
#define OBJ_BUZZBOMBER_H

#include "Game.h"

// Object Class
struct ObjectBuzzBomber {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
};

// Entity Class
struct EntityBuzzBomber {
    RSDK_ENTITY
    StateMachine(state);
    uint8 shotRange;
    int32 timer;
    uint8 detectedPlayer;
    Vector2 startPos;
    uint8 startDir;
    Entity *projectile;
    Animator animator;
    Animator wingAnimator;
    Animator thrustAnimator;
    Hitbox hitboxRange;
};

// Object Struct
extern ObjectBuzzBomber *BuzzBomber;

// Standard Entity Events
void BuzzBomber_Update(void);
void BuzzBomber_LateUpdate(void);
void BuzzBomber_StaticUpdate(void);
void BuzzBomber_Draw(void);
void BuzzBomber_Create(void *data);
void BuzzBomber_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BuzzBomber_EditorDraw(void);
void BuzzBomber_EditorLoad(void);
#endif
void BuzzBomber_Serialize(void);

// Extra Entity Functions
void BuzzBomber_DebugDraw(void);
void BuzzBomber_DebugSpawn(void);

void BuzzBomber_CheckOffScreen(void);
void BuzzBomber_CheckPlayerCollisions(void);

// States
void BuzzBomber_State_Init(void);
void BuzzBomber_State_Flying(void);
void BuzzBomber_State_Idle(void);
void BuzzBomber_State_DetectedPlayer(void);

// Projectile States
void BuzzBomber_State_ProjectileCharge(void);
void BuzzBomber_State_ProjectileShot(void);

#endif //! OBJ_BUZZBOMBER_H
