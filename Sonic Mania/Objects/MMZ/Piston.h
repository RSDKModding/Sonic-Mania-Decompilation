#ifndef OBJ_PISTON_H
#define OBJ_PISTON_H

#include "SonicMania.h"

// Object Class
struct ObjectPiston {
    RSDK_OBJECT
	uint16 landSFX;
	uint16 launchSFX;
};

// Entity Class
struct EntityPiston {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    uint32 type;
    Vector2 amplitude;
    int32 speed;
    bool32 reverse;
    uint8 size;
    uint8 collisionType;
    Vector2 targetPos;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int32 stood;
    int32 moveTimer;
    int32 stoodAngle;
    uint8 stoodPlayers;
    uint8 pushPlayersL;
    uint8 pushPlayersR;
    uint8 field_A7;
    Hitbox hitbox;
    Animator animator;
    uint32 childCount;
    uint16 interval;
    uint16 intervalOffset;
    int32 distance;
    int32 spawnType;
};

// Object Struct
extern ObjectPiston *Piston;

// Standard Entity Events
void Piston_Update(void);
void Piston_LateUpdate(void);
void Piston_StaticUpdate(void);
void Piston_Draw(void);
void Piston_Create(void *data);
void Piston_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Piston_EditorDraw(void);
void Piston_EditorLoad(void);
#endif
void Piston_Serialize(void);

// Extra Entity Functions

void Piston_WaitForInterval(void);

void Piston_Down(void);
void Piston_Up(void);
void Piston_DownOrUp(void);
void Piston_UpOrDown(void);

void Piston_Launch(void);
void Piston_Pullback(void);

void Piston_StateCollide_Solid(void);

void Piston_WaitForPlayers(void);
void Piston_PrepareLaunch(void);
void Piston_PullbackOrLaunch(void);
void Piston_LaunchAndWait(void);

void Piston_Right(void);
void Piston_Left(void);
void Piston_RightOrLeft(void);
void Piston_LeftOrRight(void);

#endif //! OBJ_PISTON_H
