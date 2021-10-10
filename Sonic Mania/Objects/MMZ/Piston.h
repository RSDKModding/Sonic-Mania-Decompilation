#ifndef OBJ_PISTON_H
#define OBJ_PISTON_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
	ushort landSFX;
	ushort launchSFX;
} ObjectPiston;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    uint type;
    Vector2 amplitude;
    int speed;
    bool32 reverse;
    byte size;
    byte collisionType;
    Vector2 targetPos;
    Vector2 centerPos;
    Vector2 drawPos;
    Vector2 collisionOffset;
    int stood;
    int moveTimer;
    int stoodAngle;
    byte stoodPlayers;
    byte pushPlayersL;
    byte pushPlayersR;
    byte field_A7;
    Hitbox hitbox;
    Animator animator;
    uint childCount;
    ushort interval;
    ushort intervalOffset;
    int distance;
    int spawnType;
} EntityPiston;

// Object Struct
extern ObjectPiston *Piston;

// Standard Entity Events
void Piston_Update(void);
void Piston_LateUpdate(void);
void Piston_StaticUpdate(void);
void Piston_Draw(void);
void Piston_Create(void *data);
void Piston_StageLoad(void);
void Piston_EditorDraw(void);
void Piston_EditorLoad(void);
void Piston_Serialize(void);

// Extra Entity Functions

void Piston_WaitForInterval(void);

void Piston_Down(void);
void Piston_Up(void);
void Piston_DownOrUp(void);
void Piston_UpOrDown(void);

void Piston_Launch(void);
void Piston_Pullback(void);

void Piston_MightyAntiHammer(void);

void Piston_WaitForPlayers(void);
void Piston_PrepareLaunch(void);
void Piston_PullbackOrLaunch(void);
void Piston_LaunchAndWait(void);

void Piston_Right(void);
void Piston_Left(void);
void Piston_RightOrLeft(void);
void Piston_LeftOrRight(void);

#endif //! OBJ_PISTON_H
