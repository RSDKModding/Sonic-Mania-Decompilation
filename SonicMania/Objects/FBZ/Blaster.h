#ifndef OBJ_BLASTER_H
#define OBJ_BLASTER_H

#include "Game.h"

typedef enum {
    BLASTER_BADNIK,
    BLASTER_SHOT,
    BLASTER_SHELL,
} BlasterTypes;

// Object Class
struct ObjectBlaster {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxRange;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
};

// Entity Class
struct EntityBlaster {
    RSDK_ENTITY
    StateMachine(state);
    uint16 unused1;
    uint16 timer;
    uint16 attackTimer;
    Vector2 startPos;
    uint8 startDir;
    Animator animator;
};

// Object Struct
extern ObjectBlaster *Blaster;

// Standard Entity Events
void Blaster_Update(void);
void Blaster_LateUpdate(void);
void Blaster_StaticUpdate(void);
void Blaster_Draw(void);
void Blaster_Create(void *data);
void Blaster_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Blaster_EditorDraw(void);
void Blaster_EditorLoad(void);
#endif
void Blaster_Serialize(void);

// Extra Entity Functions
void Blaster_DebugSpawn(void);
void Blaster_DebugDraw(void);

void Blaster_HandlePlayerInteractions(void);
void Blaster_CheckOffScreen(void);

void Blaster_State_Init(void);
void Blaster_State_Move(void);
void Blaster_State_HandleTurn(void);
void Blaster_State_AttackPlayer(void);
void Blaster_State_MagnetAttract(void);
void Blaster_State_MagnetReleased(void);
void Blaster_State_BeginShot(void);
void Blaster_State_Shot(void);
void Blaster_State_Fall(void);
void Blaster_State_Shell(void);

#endif //! OBJ_BLASTER_H
