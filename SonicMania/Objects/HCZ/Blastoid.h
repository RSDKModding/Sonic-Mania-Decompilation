#ifndef OBJ_BLASTOID_H
#define OBJ_BLASTOID_H

#include "Game.h"

// Object Class
struct ObjectBlastoid {
    RSDK_OBJECT
    Hitbox hitboxBody;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
    uint16 sfxShot;
};

// Entity Class
struct EntityBlastoid {
    RSDK_ENTITY
    StateMachine(state);
    uint8 timer;
    Animator animator;
};

// Object Struct
extern ObjectBlastoid *Blastoid;

// Standard Entity Events
void Blastoid_Update(void);
void Blastoid_LateUpdate(void);
void Blastoid_StaticUpdate(void);
void Blastoid_Draw(void);
void Blastoid_Create(void *data);
void Blastoid_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Blastoid_EditorDraw(void);
void Blastoid_EditorLoad(void);
#endif
void Blastoid_Serialize(void);

// Extra Entity Functions
void Blastoid_DebugSpawn(void);
void Blastoid_DebugDraw(void);

void Blastoid_CheckPlayerCollisions(void);

void Blastoid_State_Init(void);
void Blastoid_State_Body(void);
void Blastoid_State_Projectile(void);

#endif //! OBJ_BLASTOID_H
