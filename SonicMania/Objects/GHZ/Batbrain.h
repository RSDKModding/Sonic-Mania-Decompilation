#ifndef OBJ_BATBRAIN_H
#define OBJ_BATBRAIN_H

#include "Game.h"

// Object Class
struct ObjectBatbrain {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    uint16 aniFrames;
    uint16 sfxFlap;
};

// Entity Class
struct EntityBatbrain {
    RSDK_ENTITY
    StateMachine(state);
    int32 targetY;
    EntityPlayer *target;
    Vector2 startPos;
    Animator animator;
};

// Object Struct
extern ObjectBatbrain *Batbrain;

// Standard Entity Events
void Batbrain_Update(void);
void Batbrain_LateUpdate(void);
void Batbrain_StaticUpdate(void);
void Batbrain_Draw(void);
void Batbrain_Create(void *data);
void Batbrain_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Batbrain_EditorDraw(void);
void Batbrain_EditorLoad(void);
#endif
void Batbrain_Serialize(void);

// Extra Entity Functions
void Batbrain_DebugDraw(void);
void Batbrain_DebugSpawn(void);

// Helpers
void Batbrain_CheckPlayerCollisions(void);
void Batbrain_CheckOffScreen(void);

// States
void Batbrain_State_Init(void);
void Batbrain_State_CheckPlayerInRange(void);
void Batbrain_State_DropToPlayer(void);
void Batbrain_State_Fly(void);
void Batbrain_State_FlyToCeiling(void);

#endif //! OBJ_BATBRAIN_H
