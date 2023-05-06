#ifndef OBJ_PINATA_H
#define OBJ_PINATA_H

#include "Game.h"

typedef enum { PINATA_PRIO_HIGH, PINATA_PRIO_LOW } PinataPriorities;

// Object Class
struct ObjectPinata {
    RSDK_OBJECT
    Hitbox hitboxPinata;
    uint16 aniFrames;
    uint16 sfxPinata;
};

// Entity Class
struct EntityPinata {
    RSDK_ENTITY
    StateMachine(state);
    bool32 respawn;
    int32 priority;
    Animator animator;
};

// Object Struct
extern ObjectPinata *Pinata;

// Standard Entity Events
void Pinata_Update(void);
void Pinata_LateUpdate(void);
void Pinata_StaticUpdate(void);
void Pinata_Draw(void);
void Pinata_Create(void *data);
void Pinata_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Pinata_EditorDraw(void);
void Pinata_EditorLoad(void);
#endif
void Pinata_Serialize(void);

// Extra Entity Functions
void Pinata_DebugDraw(void);
void Pinata_DebugSpawn(void);

void Pinata_State_CheckPlayerCollisions(void);
void Pinata_State_Destroyed(void);

#endif //! OBJ_PINATA_H
