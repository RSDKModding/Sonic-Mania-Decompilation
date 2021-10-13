#ifndef OBJ_CLUCKER_H
#define OBJ_CLUCKER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxRange;
    Hitbox hitboxSolid;
    Hitbox hitbox4;
    uint16 aniFrames;
    uint16 sfxShot;
} ObjectClucker;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int yOffset;
    int field_60;
    uint8 timer;
    uint16 delay;
    bool32 hasChildren;
    Animator animator;
} EntityClucker;

// Object Struct
extern ObjectClucker *Clucker;

// Standard Entity Events
void Clucker_Update(void);
void Clucker_LateUpdate(void);
void Clucker_StaticUpdate(void);
void Clucker_Draw(void);
void Clucker_Create(void* data);
void Clucker_StageLoad(void);
void Clucker_EditorDraw(void);
void Clucker_EditorLoad(void);
void Clucker_Serialize(void);

// Extra Entity Functions
void Clucker_DebugSpawn(void);
void Clucker_DebugDraw(void);

void Clucker_HandlePlayerInteractions(void);
void Clucker_CheckOnScreen(void);

void Clucker_State_Setup(void);
void Clucker_State_Unknown1(void);
void Clucker_State_Unknown2(void);
void Clucker_State_Unknown3(void);
void Clucker_State_Unknown4(void);
void Clucker_State_Unknown5(void);
void Clucker_State_Destroyed(void);

void Clucker_State_Projectile(void);

#endif //!OBJ_CLUCKER_H
