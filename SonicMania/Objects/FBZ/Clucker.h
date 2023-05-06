#ifndef OBJ_CLUCKER_H
#define OBJ_CLUCKER_H

#include "Game.h"

// Object Class
struct ObjectClucker {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxRange;
    Hitbox hitboxSolid;
    Hitbox hitboxEgg;
    uint16 aniFrames;
    uint16 sfxShot;
};

// Entity Class
struct EntityClucker {
    RSDK_ENTITY
    StateMachine(state);
    int32 yOffset;
    int32 xOffset;
    uint8 timer;
    uint16 delay;
    bool32 hasParent;
    Animator animator;
};

// Object Struct
extern ObjectClucker *Clucker;

// Standard Entity Events
void Clucker_Update(void);
void Clucker_LateUpdate(void);
void Clucker_StaticUpdate(void);
void Clucker_Draw(void);
void Clucker_Create(void *data);
void Clucker_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Clucker_EditorDraw(void);
void Clucker_EditorLoad(void);
#endif
void Clucker_Serialize(void);

// Extra Entity Functions
void Clucker_DebugSpawn(void);
void Clucker_DebugDraw(void);

void Clucker_HandlePlayerInteractions(void);
void Clucker_CheckOffScreen(void);

void Clucker_State_Init(void);
void Clucker_State_CheckForPlayer(void);
void Clucker_State_Appear(void);
void Clucker_State_ShootDelay(void);
void Clucker_State_Shoot(void);
void Clucker_State_Turn(void);
void Clucker_State_Destroyed(void);

void Clucker_State_Egg(void);

#endif //! OBJ_CLUCKER_H
