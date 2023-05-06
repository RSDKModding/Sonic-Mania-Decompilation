#ifndef OBJ_IWAMODOKI_H
#define OBJ_IWAMODOKI_H

#include "Game.h"

// Object Class
struct ObjectIwamodoki {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxRange;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
};

// Entity Class
struct EntityIwamodoki {
    RSDK_ENTITY
    StateMachine(state);
    bool32 lrzConvPhys;
    Vector2 startPos;
    Vector2 moveOffset;
    Vector2 preMovePos;
    Vector2 postMovePos;
    uint8 startDir;
    int32 timer;
    int32 chargeCount;
    Animator animator;
};

// Object Struct
extern ObjectIwamodoki *Iwamodoki;

// Standard Entity Events
void Iwamodoki_Update(void);
void Iwamodoki_LateUpdate(void);
void Iwamodoki_StaticUpdate(void);
void Iwamodoki_Draw(void);
void Iwamodoki_Create(void *data);
void Iwamodoki_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Iwamodoki_EditorDraw(void);
void Iwamodoki_EditorLoad(void);
#endif
void Iwamodoki_Serialize(void);

// Extra Entity Functions
void Iwamodoki_DebugSpawn(void);
void Iwamodoki_DebugDraw(void);

void Iwamodoki_HandlePlayerCollisions(void);
void Iwamodoki_CheckOffScreen(void);

void Iwamodoki_State_Init(void);
void Iwamodoki_State_AwaitPlayer(void);
void Iwamodoki_State_Appear(void);
void Iwamodoki_State_Charging(void);
void Iwamodoki_State_Explode(void);
void Iwamodoki_State_Debris(void);

#endif //! OBJ_IWAMODOKI_H
