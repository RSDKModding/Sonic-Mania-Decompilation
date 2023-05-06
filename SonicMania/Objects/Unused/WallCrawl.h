#ifndef OBJ_WALLCRAWL_H
#define OBJ_WALLCRAWL_H

#include "Game.h"

// Object Class
struct ObjectWallCrawl {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxRange;
    Hitbox hitboxLaser;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
};

// Entity Class
struct EntityWallCrawl {
    RSDK_ENTITY
    StateMachine(state);
    int32 idleTimer;
    int32 timer;
    EntityPlayer *playerPtr;
    Vector2 startPos;
    uint8 startDir;
    Animator animator;
};

// Object Entity
extern ObjectWallCrawl *WallCrawl;

// Standard Entity Events
void WallCrawl_Update(void);
void WallCrawl_LateUpdate(void);
void WallCrawl_StaticUpdate(void);
void WallCrawl_Draw(void);
void WallCrawl_Create(void *data);
void WallCrawl_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void WallCrawl_EditorDraw(void);
void WallCrawl_EditorLoad(void);
#endif
void WallCrawl_Serialize(void);

// Extra Entity Functions
void WallCrawl_DebugSpawn(void);
void WallCrawl_DebugDraw(void);

void WallCrawl_HandlePlayerInteractions(void);
void WallCrawl_CheckOffScreen(void);

void WallCrawl_State_Init(void);
void WallCrawl_State_Moving(void);
void WallCrawl_State_Idle(void);

void WallCrawl_State_Projectile(void);

#endif //! OBJ_WALLCRAWL_H
