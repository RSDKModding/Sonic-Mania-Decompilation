#ifndef OBJ_SCARAB_H
#define OBJ_SCARAB_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxGrab;
    uint16 aniFrames;
} ObjectScarab;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    Vector2 moveOffset;
    uint8 startDir;
    int timer;
    int playerTimers[4];
    Animator animator1;
    Animator animator3;
    Animator animator2;
    int amplitude;
    uint8 childCount;
    PlaneFilterTypes planeFilter;
    uint8 drawOrderLow;
    uint8 drawOrderHigh;
    uint8 timer2;
    int8 field_DB;
    uint8 grabbedPlayers;
} EntityScarab;

// Object Struct
extern ObjectScarab *Scarab;

// Standard Entity Events
void Scarab_Update(void);
void Scarab_LateUpdate(void);
void Scarab_StaticUpdate(void);
void Scarab_Draw(void);
void Scarab_Create(void* data);
void Scarab_StageLoad(void);
void Scarab_EditorDraw(void);
void Scarab_EditorLoad(void);
void Scarab_Serialize(void);

// Extra Entity Functions
void Scarab_DebugSpawn(void);
void Scarab_DebugDraw(void);

void Scarab_CheckPlayerCollisions(void);
void Scarab_HandleChildMove(void);
void Scarab_HandlePlayerGrab(void);
void Scarab_HandlePlayerRelease(void);

void Scarab_State_Setup(void);
void Scarab_State_Move(void);
void Scarab_State_Wait(void);

#endif //!OBJ_SCARAB_H
