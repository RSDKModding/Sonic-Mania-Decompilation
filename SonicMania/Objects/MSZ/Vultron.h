#ifndef OBJ_VULTRON_H
#define OBJ_VULTRON_H

#include "Game.h"

typedef enum {
    VULTRON_DIVE,
    VULTRON_TARGET,
} VultronTypes;

// Object Class
struct ObjectVultron {
    RSDK_OBJECT
    Hitbox hitboxRange;
    uint16 aniFrames;
    uint16 sfxVultron;
};

// Entity Class
struct EntityVultron {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    Vector2 startPos;
    uint8 startDir;
    uint16 dist;
    uint16 distRemain;
    int32 storeY;
    Hitbox hitboxBadnik;
    Animator bodyAnimator;
    Animator flameAnimator;
};

// Object Struct
extern ObjectVultron *Vultron;

// Standard Entity Events
void Vultron_Update(void);
void Vultron_LateUpdate(void);
void Vultron_StaticUpdate(void);
void Vultron_Draw(void);
void Vultron_Create(void *data);
void Vultron_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Vultron_EditorDraw(void);
void Vultron_EditorLoad(void);
#endif
void Vultron_Serialize(void);

// Extra Entity Functions
void Vultron_DebugSpawn(void);
void Vultron_DebugDraw(void);

void Vultron_CheckPlayerCollisions(void);
void Vultron_CheckOffScreen(void);

void Vultron_State_Init(void);
void Vultron_State_CheckPlayerInRange(void);
void Vultron_State_Hop(void);
void Vultron_State_Dive(void);
void Vultron_State_Flying(void);
void Vultron_State_Rise(void);
void Vultron_State_PrepareDive(void);

void Vultron_State_Targeting(void);

#endif //! OBJ_VULTRON_H
