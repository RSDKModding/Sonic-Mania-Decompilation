#ifndef OBJ_AQUIS_H
#define OBJ_AQUIS_H

#include "Game.h"

// Object Class
struct ObjectAquis {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxRange;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
    uint16 sfxShot;
};

// Entity Class
struct EntityAquis {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    uint8 playerInRange;
    int32 remainingTurns;
    Vector2 startPos;
    uint8 startDir;
    Animator mainAnimator;
    Animator wingAnimator;
};

// Object Struct
extern ObjectAquis *Aquis;

// Standard Entity Events
void Aquis_Update(void);
void Aquis_LateUpdate(void);
void Aquis_StaticUpdate(void);
void Aquis_Draw(void);
void Aquis_Create(void *data);
void Aquis_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Aquis_EditorDraw(void);
void Aquis_EditorLoad(void);
#endif
void Aquis_Serialize(void);

// Extra Entity Functions
void Aquis_DebugSpawn(void);
void Aquis_DebugDraw(void);

void Aquis_CheckPlayerCollisions(void);
void Aquis_CheckOffScreen(void);

void Aquis_State_Init(void);
void Aquis_State_Idle(void);
void Aquis_State_Moving(void);
void Aquis_State_Shoot(void);
void Aquis_State_Turning(void);
void Aquis_State_Flee(void);

void Aquis_State_Shot(void);

#endif //! OBJ_AQUIS_H
