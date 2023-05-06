#ifndef OBJ_SPINY_H
#define OBJ_SPINY_H

#include "Game.h"

typedef enum {
    SPINY_FLOOR,
    SPINY_WALL,
} SpinyTypes;

// Object Class
struct ObjectSpiny {
    RSDK_OBJECT
    Hitbox hitboxSpiny;
    Hitbox hitboxRange;
    Hitbox hitboxShot;
    uint16 aniFrames;
    uint16 sfxShot;
};

// Entity Class
struct EntitySpiny {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator;
    int32 unused;
    uint8 type;
    Vector2 startPos;
    uint8 startDir;
    int32 shotVelocity;
    int32 dirChangeTimer;
    int32 moveTimer;
};

// Object Struct
extern ObjectSpiny *Spiny;

// Standard Entity Events
void Spiny_Update(void);
void Spiny_LateUpdate(void);
void Spiny_StaticUpdate(void);
void Spiny_Draw(void);
void Spiny_Create(void *data);
void Spiny_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Spiny_EditorDraw(void);
void Spiny_EditorLoad(void);
#endif
void Spiny_Serialize(void);

// Extra Entity Functions
void Spiny_DebugSpawn(void);
void Spiny_DebugDraw(void);

void Spiny_CheckPlayerCollisions(void);
void Spiny_CheckOffScreen(void);

void Spiny_State_Init(void);
void Spiny_State_Floor(void);
void Spiny_State_Shoot_Floor(void);
void Spiny_State_Wall(void);
void Spiny_State_Shoot_Wall(void);

void Spiny_State_Shot(void);
void Spiny_State_ShotDisappear(void);

#endif //! OBJ_SPINY_H
