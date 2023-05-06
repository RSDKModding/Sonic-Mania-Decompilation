#ifndef OBJ_TOXOMISTER_H
#define OBJ_TOXOMISTER_H

#include "Game.h"

// Object Class
struct ObjectToxomister {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxCloud;
    uint16 aniFrames;
};

// Entity Class
struct EntityToxomister {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    EntityToxomister *parent;
    EntityPlayer *grabbedPlayer;
    int32 timer;
    uint16 shakeTimer;
    uint16 shakeCount;
    uint16 prevShakeFlags;
    Animator animator;
};

// Object Struct
extern ObjectToxomister *Toxomister;

// Standard Entity Events
void Toxomister_Update(void);
void Toxomister_LateUpdate(void);
void Toxomister_StaticUpdate(void);
void Toxomister_Draw(void);
void Toxomister_Create(void *data);
void Toxomister_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Toxomister_EditorDraw(void);
void Toxomister_EditorLoad(void);
#endif
void Toxomister_Serialize(void);

// Extra Entity Functions
void Toxomister_DebugSpawn(void);
void Toxomister_DebugDraw(void);

void Toxomister_CheckPlayerCollisions(void);
void Toxomister_CheckOffScreen(void);
void Toxomister_CheckCloudOnScreen(void);
void Toxomister_CheckPlayerCloudCollisions(void);

void Toxomister_State_Init(void);
void Toxomister_State_CreateClouds(void);

void Toxomister_StateCloud_FallDelay(void);
void Toxomister_StateCloud_FallToFloor(void);
void Toxomister_StateCloud_ReachedFloor(void);
void Toxomister_StateCloud_GrabbedPlayer(void);
void Toxomister_StateCloud_Dissipate(void);

#endif //! OBJ_TOXOMISTER_H
