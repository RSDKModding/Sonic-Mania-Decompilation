#ifndef OBJ_FIREBALL_H
#define OBJ_FIREBALL_H

#include "Game.h"

typedef enum {
    FIREBALL_SPAWNER,
    FIREBALL_LAUNCHER_STATIC,
    FIREBALL_LAUNCHER_GRAVITY,
} FireballTypes;

// Object Class
struct ObjectFireball {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxFireball;
    uint16 sfxFireball;
};

// Entity Class
struct EntityFireball {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Animator animator;
    uint8 type;
    uint8 interval;
    uint8 intervalOffset;
    int32 unused1;
    int32 unused2;
};

// Object Entity
extern ObjectFireball *Fireball;

// Standard Entity Events
void Fireball_Update(void);
void Fireball_LateUpdate(void);
void Fireball_StaticUpdate(void);
void Fireball_Draw(void);
void Fireball_Create(void *data);
void Fireball_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Fireball_EditorDraw(void);
void Fireball_EditorLoad(void);
#endif
void Fireball_Serialize(void);

// Extra Entity Functions
void Fireball_HandlePlayerInteractions(void);

// Spawner States
void Fireball_State_Spawner(void);
void Fireball_State_LauncherStatic(void);
void Fireball_State_LauncherGravity(void);

// Fireball States
void Fireball_StateFireball_Spawner(void);
void Fireball_StateFireball_LauncherStatic(void);
void Fireball_StateFireball_LauncherGravity(void);
void Fireball_StateFireball_Dissipate(void);

void Fireball_Draw_Simple(void);

#endif //! OBJ_FIREBALL_H
