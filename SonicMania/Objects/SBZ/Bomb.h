#ifndef OBJ_BOMB_H
#define OBJ_BOMB_H

#include "Game.h"

// Object Class
struct ObjectBomb {
    RSDK_OBJECT
    Hitbox hitboxHurt;
    Hitbox hitboxRange;
    Hitbox hitboxShrapnel;
    uint16 aniFrames;
    uint16 sfxExplosion;
};

// Entity Class
struct EntityBomb {
    RSDK_ENTITY
    StateMachine(state);
    int32 planeFilter;
    Vector2 startPos;
    uint8 startDir;
    int32 timer;
    int32 fuseOffset;
    Animator mainAnimator;
    Animator fuseAnimator;
};

// Object Entity
extern ObjectBomb *Bomb;

// Standard Entity Events
void Bomb_Update(void);
void Bomb_LateUpdate(void);
void Bomb_StaticUpdate(void);
void Bomb_Draw(void);
void Bomb_Create(void *data);
void Bomb_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Bomb_EditorDraw(void);
void Bomb_EditorLoad(void);
#endif
void Bomb_Serialize(void);

// Extra Entity Functions
void Bomb_DebugSpawn(void);
void Bomb_DebugDraw(void);

void Bomb_CheckOffScreen(void);
void Bomb_CheckPlayerCollisions(void);

void Bomb_State_Init(void);
void Bomb_State_Walk(void);
void Bomb_State_Idle(void);
void Bomb_State_Explode(void);
void Bomb_State_Shrapnel(void);

#endif //! OBJ_BOMB_H
