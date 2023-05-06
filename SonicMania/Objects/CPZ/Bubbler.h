#ifndef OBJ_BUBBLER_H
#define OBJ_BUBBLER_H

#include "Game.h"

// Object Class
struct ObjectBubbler {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxRange;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
};

// Entity Class
struct EntityBubbler {
    RSDK_ENTITY
    StateMachine(state);
    int16 timer;
    uint16 spawnTimer;
    Vector2 startPos;
    uint8 startDir;
    Animator bodyHitbox;
    Animator flameAnimator;
};

// Object Entity
extern ObjectBubbler *Bubbler;

// Standard Entity Events
void Bubbler_Update(void);
void Bubbler_LateUpdate(void);
void Bubbler_StaticUpdate(void);
void Bubbler_Draw(void);
void Bubbler_Create(void *data);
void Bubbler_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Bubbler_EditorDraw(void);
void Bubbler_EditorLoad(void);
#endif
void Bubbler_Serialize(void);

// Extra Entity Functions
void Bubbler_DebugSpawn(void);
void Bubbler_DebugDraw(void);

void Bubbler_HandleInteractions(void);
void Bubbler_HandleProjectileInteractions(void);
void Bubbler_CheckOffScreen(void);

void Bubbler_State_Init(void);
void Bubbler_State_MotherPatrol(void);
void Bubbler_State_FoundPlayer(void);
void Bubbler_State_AttackPlayer(void);

void Bubbler_StateProjectile_Seed(void);
void Bubbler_StateProjectile_Bubbler(void);

#endif //! OBJ_BUBBLER_H
