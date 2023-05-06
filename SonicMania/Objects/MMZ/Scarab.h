#ifndef OBJ_SCARAB_H
#define OBJ_SCARAB_H

#include "Game.h"

// Object Class
struct ObjectScarab {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxGrab;
    uint16 aniFrames;
};

// Entity Class
struct EntityScarab {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    Vector2 moveOffset;
    uint8 startDir;
    int32 timer;
    int32 playerTimers[PLAYER_COUNT];
    Animator bodyAnimator;
    Animator frontLegAnimator;
    Animator backLegAnimator;
    int32 amplitude;
    uint8 childCount;
    PlaneFilterTypes planeFilter;
    uint8 drawGroupLow;
    uint8 drawGroupHigh;
    uint8 pullCount;
    int8 moveDir;
    uint8 grabbedPlayers;
};

// Object Struct
extern ObjectScarab *Scarab;

// Standard Entity Events
void Scarab_Update(void);
void Scarab_LateUpdate(void);
void Scarab_StaticUpdate(void);
void Scarab_Draw(void);
void Scarab_Create(void *data);
void Scarab_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Scarab_EditorDraw(void);
void Scarab_EditorLoad(void);
#endif
void Scarab_Serialize(void);

// Extra Entity Functions
void Scarab_DebugSpawn(void);
void Scarab_DebugDraw(void);

void Scarab_CheckPlayerCollisions(void);
void Scarab_HandleChildMove(void);
void Scarab_HandlePlayerGrab(void);
void Scarab_HandlePlayerRelease(void);

void Scarab_State_Init(void);
void Scarab_State_Move(void);
void Scarab_State_Wait(void);

#endif //! OBJ_SCARAB_H
