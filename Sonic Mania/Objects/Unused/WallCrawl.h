#ifndef OBJ_WALLCRAWL_H
#define OBJ_WALLCRAWL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
} ObjectWallCrawl;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int idleTimer;
    int timer;
    EntityPlayer *playerPtr;
    Vector2 startPos;
    uint8 startDir;
    Animator animator;
} EntityWallCrawl;

// Object Entity
extern ObjectWallCrawl *WallCrawl;

// Standard Entity Events
void WallCrawl_Update(void);
void WallCrawl_LateUpdate(void);
void WallCrawl_StaticUpdate(void);
void WallCrawl_Draw(void);
void WallCrawl_Create(void* data);
void WallCrawl_StageLoad(void);
void WallCrawl_EditorDraw(void);
void WallCrawl_EditorLoad(void);
void WallCrawl_Serialize(void);

// Extra Entity Functions
void WallCrawl_DebugSpawn(void);
void WallCrawl_DebugDraw(void);

void WallCrawl_HandlePlayerInteractions(void);
void WallCrawl_CheckOnScreen(void);

void WallCrawl_State_Setup(void);
void WallCrawl_State_Main(void);
void WallCrawl_State_Idle(void);

void WallCrawl_State_Projectile(void);

#endif //!OBJ_WALLCRAWL_H
