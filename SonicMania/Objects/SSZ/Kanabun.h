#ifndef OBJ_KANABUN_H
#define OBJ_KANABUN_H

#include "Game.h"

// Object Class
struct ObjectKanabun {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    uint16 aniFrames;
};

// Entity Class
struct EntityKanabun {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused;
    Vector2 startPos;
    uint8 startDir;
    int32 hVel;
    uint8 hDist;
    uint8 bobDist;
    uint8 angleVel;
    Animator animator;
};

// Object Struct
extern ObjectKanabun *Kanabun;

// Standard Entity Events
void Kanabun_Update(void);
void Kanabun_LateUpdate(void);
void Kanabun_StaticUpdate(void);
void Kanabun_Draw(void);
void Kanabun_Create(void *data);
void Kanabun_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Kanabun_EditorDraw(void);
void Kanabun_EditorLoad(void);
#endif
void Kanabun_Serialize(void);

// Extra Entity Functions
void Kanabun_DebugSpawn(void);
void Kanabun_DebugDraw(void);

void Kanabun_CheckPlayerCollisions(void);
void Kanabun_CheckOffScreen(void);
void Kanabun_HandleMovement(void);

void Kanabun_State_Init(void);
void Kanabun_State_Moving(void);
void Kanabun_State_Turning(void);

#endif //! OBJ_KANABUN_H
