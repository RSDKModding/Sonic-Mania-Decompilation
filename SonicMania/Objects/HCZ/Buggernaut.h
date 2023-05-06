#ifndef OBJ_BUGGERNAUT_H
#define OBJ_BUGGERNAUT_H

#include "Game.h"

// Object Class
struct ObjectBuggernaut {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxParentRange;
    uint16 aniFrames;
};

// Entity Class
struct EntityBuggernaut {
    RSDK_ENTITY
    StateMachine(state);
    bool32 passThrough;
    int32 timer;
    int32 buzzCount;
    Vector2 startPos;
    EntityBuggernaut *parent;
    Animator bodyAnimator;
    Animator wingAnimator;
};

// Object Struct
extern ObjectBuggernaut *Buggernaut;

// Standard Entity Events
void Buggernaut_Update(void);
void Buggernaut_LateUpdate(void);
void Buggernaut_StaticUpdate(void);
void Buggernaut_Draw(void);
void Buggernaut_Create(void *data);
void Buggernaut_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Buggernaut_EditorDraw(void);
void Buggernaut_EditorLoad(void);
#endif
void Buggernaut_Serialize(void);

// Extra Entity Functions
void Buggernaut_DebugSpawn(void);
void Buggernaut_DebugDraw(void);

void Buggernaut_CheckPlayerCollisions(void);
void Buggernaut_CheckOffScreen(void);
bool32 Buggernaut_HandleTileCollisionsX(void);
bool32 Buggernaut_HandleTileCollisionsY(void);

void Buggernaut_State_Init(void);
void Buggernaut_State_Idle(void);
void Buggernaut_State_FlyTowardTarget(void);
void Buggernaut_State_FlyAway(void);

void Buggernaut_State_Child(void);

#endif //! OBJ_BUGGERNAUT_H
