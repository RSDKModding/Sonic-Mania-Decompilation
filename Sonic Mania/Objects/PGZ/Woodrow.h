#ifndef OBJ_WOODROW_H
#define OBJ_WOODROW_H

#include "SonicMania.h"

typedef enum { WOODROW_BADNIK, WOODROW_BOMB } WoodrowTypes;

// Object Class
struct ObjectWoodrow {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    uint16 aniFrames;
    uint16 sfxExplosion;
    uint16 sfxPeck;
};

// Entity Class
struct EntityWoodrow {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    int32 distUp;
    int32 distDown;
    uint8 activeBombCount;
    uint8 bombCount;
    uint8 bombFallDelay;
    uint8 moveDelay;
    int32 collisionFlags;
    Vector2 startPos;
    uint8 startDir;
    Entity *parent;
    Animator animator;
};

// Object Struct
extern ObjectWoodrow *Woodrow;

// Standard Entity Events
void Woodrow_Update(void);
void Woodrow_LateUpdate(void);
void Woodrow_StaticUpdate(void);
void Woodrow_Draw(void);
void Woodrow_Create(void* data);
void Woodrow_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Woodrow_EditorDraw(void);
void Woodrow_EditorLoad(void);
#endif
void Woodrow_Serialize(void);

// Extra Entity Functions
void Woodrow_DebugSpawn(void);
void Woodrow_DebugDraw(void);

void Woodrow_CheckPlayerCollisions(void);
void Woodrow_CheckOnScreen(void);

void Woodrow_State_Setup(void);
void Woodrow_Unknown4(void);
void Woodrow_Unknown5(void);
void Woodrow_Unknown6(void);
void Woodrow_State_Bomb(void);
void Woodrow_State_BombSpawner(void);

#endif //!OBJ_WOODROW_H
