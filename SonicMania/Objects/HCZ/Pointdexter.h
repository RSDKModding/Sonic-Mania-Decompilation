#ifndef OBJ_POINTDEXTER_H
#define OBJ_POINTDEXTER_H

#include "Game.h"

// Object Class
struct ObjectPointdexter {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
};

// Entity Class
struct EntityPointdexter {
    RSDK_ENTITY
    StateMachine(state);
    uint8 timer;
    Vector2 startPos;
    uint8 startDir;
    Animator animator;
};

// Object Struct
extern ObjectPointdexter *Pointdexter;

// Standard Entity Events
void Pointdexter_Update(void);
void Pointdexter_LateUpdate(void);
void Pointdexter_StaticUpdate(void);
void Pointdexter_Draw(void);
void Pointdexter_Create(void *data);
void Pointdexter_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Pointdexter_EditorDraw(void);
void Pointdexter_EditorLoad(void);
#endif
void Pointdexter_Serialize(void);

// Extra Entity Functions
void Pointdexter_DebugSpawn(void);
void Pointdexter_DebugDraw(void);

void Pointdexter_CheckOffScreen(void);
void Pointdexter_CheckPlayerCollisions(void);

void Pointdexter_State_Init(void);
void Pointdexter_State_Swimming(void);

#endif //! OBJ_POINTDEXTER_H
