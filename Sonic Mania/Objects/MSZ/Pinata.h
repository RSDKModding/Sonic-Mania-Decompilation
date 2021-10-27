#ifndef OBJ_PINATA_H
#define OBJ_PINATA_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxPinata;
} ObjectPinata;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    bool32 respawn;
    int priority;
    Animator animator;
} EntityPinata;

// Object Struct
extern ObjectPinata *Pinata;

// Standard Entity Events
void Pinata_Update(void);
void Pinata_LateUpdate(void);
void Pinata_StaticUpdate(void);
void Pinata_Draw(void);
void Pinata_Create(void* data);
void Pinata_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Pinata_EditorDraw(void);
void Pinata_EditorLoad(void);
#endif
void Pinata_Serialize(void);

// Extra Entity Functions
void Pinata_DebugDraw(void);
void Pinata_DebugSpawn(void);

void Pinata_State_CheckPlayerCollisions(void);
void Pinata_State_Destroyed(void);

#endif //!OBJ_PINATA_H
