#ifndef OBJ_WOODROW_H
#define OBJ_WOODROW_H

#include "Game.h"

typedef enum { WOODROW_BADNIK, WOODROW_BOMB } WoodrowTypes;

// Object Class
struct ObjectWoodrow {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxFullRange;
    Hitbox hitboxBombRange;
    Hitbox hitboxBomb;
    uint16 aniFrames;
    uint16 sfxExplosion;
    uint16 sfxPeck;
};

// Entity Class
struct EntityWoodrow {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    int32 distUp;
    int32 distDown;
    uint8 activeBombCount;
    uint8 bombCount;
    uint8 bombFallDelay;
    uint8 moveDelay;
    int32 rangeMask;
    Vector2 startPos;
    uint8 startDir;
    EntityWoodrow *parent;
    Animator animator;
};

// Object Struct
extern ObjectWoodrow *Woodrow;

// Standard Entity Events
void Woodrow_Update(void);
void Woodrow_LateUpdate(void);
void Woodrow_StaticUpdate(void);
void Woodrow_Draw(void);
void Woodrow_Create(void *data);
void Woodrow_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Woodrow_EditorDraw(void);
void Woodrow_EditorLoad(void);
#endif
void Woodrow_Serialize(void);

// Extra Entity Functions
void Woodrow_DebugSpawn(void);
void Woodrow_DebugDraw(void);

void Woodrow_CheckPlayerCollisions(void);
void Woodrow_CheckOffScreen(void);

void Woodrow_State_Init(void);
void Woodrow_State_Idle(void);
void Woodrow_State_MoveUp(void);
void Woodrow_State_MoveDown(void);
void Woodrow_State_Bomb(void);
void Woodrow_State_BombSpawner(void);

#endif //! OBJ_WOODROW_H
