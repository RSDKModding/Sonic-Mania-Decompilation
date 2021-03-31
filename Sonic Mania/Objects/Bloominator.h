#ifndef OBJ_BLOOMINATOR_H
#define OBJ_BLOOMINATOR_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox projectileHitbox;
    ushort spriteIndex;
    ushort sfxShot;
} ObjectBloominator;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    int timer;
    AnimationData data;
} EntityBloominator;

// Object Struct
extern ObjectBloominator *Bloominator;

// Standard Entity Events
void Bloominator_Update();
void Bloominator_LateUpdate();
void Bloominator_StaticUpdate();
void Bloominator_Draw();
void Bloominator_Create(void* data);
void Bloominator_StageLoad();
void Bloominator_EditorDraw();
void Bloominator_EditorLoad();
void Bloominator_Serialize();

// Extra Entity Functions
void Bloominator_DebugDraw();
void Bloominator_DebugSpawn();

void Bloominator_CheckHit();
void Bloominator_Idle();
void Bloominator_State_Firing();
void Bloominator_State_Setup();
void Bloominator_State_Spikeball();
#endif

#endif //!OBJ_BLOOMINATOR_H
