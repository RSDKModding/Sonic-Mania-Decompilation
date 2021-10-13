#ifndef OBJ_BLASTOID_H
#define OBJ_BLASTOID_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitboxBody;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
    uint16 sfxShot;
} ObjectBlastoid;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 timer;
    Animator animator;
} EntityBlastoid;

// Object Struct
extern ObjectBlastoid *Blastoid;

// Standard Entity Events
void Blastoid_Update(void);
void Blastoid_LateUpdate(void);
void Blastoid_StaticUpdate(void);
void Blastoid_Draw(void);
void Blastoid_Create(void* data);
void Blastoid_StageLoad(void);
void Blastoid_EditorDraw(void);
void Blastoid_EditorLoad(void);
void Blastoid_Serialize(void);

// Extra Entity Functions
void Blastoid_DebugSpawn(void);
void Blastoid_DebugDraw(void);

void Blastoid_CheckPlayerCollisions(void);

void Blastoid_State_Setup(void);
void Blastoid_State_Body(void);
void Blastoid_State_Projectile(void);

#endif //!OBJ_BLASTOID_H
