#ifndef OBJ_CRABMEAT_H
#define OBJ_CRABMEAT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox projectileHitbox;
    ushort spriteIndex;
} ObjectCrabmeat;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void(*state)();
    int timer;
    byte dword60;
    Vector2 startPos;
    int startDir;
    AnimationData data;
} EntityCrabmeat;

// Object Struct
extern ObjectCrabmeat *Crabmeat;

// Standard Entity Events
void Crabmeat_Update();
void Crabmeat_LateUpdate();
void Crabmeat_StaticUpdate();
void Crabmeat_Draw();
void Crabmeat_Create(void* data);
void Crabmeat_StageLoad();
void Crabmeat_EditorDraw();
void Crabmeat_EditorLoad();
void Crabmeat_Serialize();

// Extra Entity Functions
void Crabmeat_DebugDraw();
void Crabmeat_DebugSpawn();

void Crabmeat_State_Main();
void Crabmeat_State_Projectile();
void Crabmeat_State_Shoot();
void Crabmeat_Unknown1();

void Crabmeat_CheckOnScreen();
void Crabmeat_CheckPlayerCollisions();

#endif //!OBJ_CRABMEAT_H
