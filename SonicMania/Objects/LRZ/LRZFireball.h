#ifndef OBJ_LRZFIREBALL_H
#define OBJ_LRZFIREBALL_H

#include "Game.h"

typedef enum {
    LRZFIREBALL_SPAWNER,
    LRZFIREBALL_LAUNCHER_STATIC,
    LRZFIREBALL_LAUNCHER_GRAVITY,
} LRZFireballTypes;

// Object Class
struct ObjectLRZFireball {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxFireball;
    uint16 sfxFireball;
};

// Entity Class
struct EntityLRZFireball {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Animator animator;
    uint8 type;
    uint16 interval;
    uint16 intervalOffset;
    int32 unused1;
    int32 unused2;
    int32 unused3;
};

// Object Struct
extern ObjectLRZFireball *LRZFireball;

// Standard Entity Events
void LRZFireball_Update(void);
void LRZFireball_LateUpdate(void);
void LRZFireball_StaticUpdate(void);
void LRZFireball_Draw(void);
void LRZFireball_Create(void *data);
void LRZFireball_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LRZFireball_EditorDraw(void);
void LRZFireball_EditorLoad(void);
#endif
void LRZFireball_Serialize(void);

// Extra Entity Functions
void LRZFireball_CheckPlayerCollisions(void);
void LRZFireball_CheckOffScreen(void);
void LRZFireball_CheckTileCollisions(void);

// Spawner States
void LRZFireball_State_Spawner(void);
void LRZFireball_State_LauncherStatic(void);
void LRZFireball_State_LauncherGravity(void);

// Fireball States
void LRZFireball_StateFireball_Spawner(void);
void LRZFireball_StateFireball_LauncherStatic(void);
void LRZFireball_StateFireball_LauncherGravity(void);

void LRZFireball_Draw_Simple(void);

#endif //! OBJ_LRZFIREBALL_H
