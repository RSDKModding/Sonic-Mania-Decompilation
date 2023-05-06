#ifndef OBJ_EGGPISTONSMKII_H
#define OBJ_EGGPISTONSMKII_H

#include "Game.h"

typedef enum {
    EGGPISTON_PISTON,
    EGGPISTON_CONTROL,
    EGGPISTON_EMITTER,
    EGGPISTON_BARRIER,
    EGGPISTON_PLASMABALL,
    EGGPISTON_ALARM,
} EggPistonMKIITypes;

// Object Class
struct ObjectEggPistonsMKII {
    RSDK_OBJECT
    int32 invincibilityTimer;
    int32 health;
    int32 alarmAngle;
    int32 alarmTimer;
    EntityEggPistonsMKII *controller;
    EntityEggPistonsMKII *pistons[5];
    uint8 pistonCount;
    uint8 isPhase2;
    uint8 pistonID;
    uint16 aniFrames;
    uint16 eggmanFrames;
    uint16 sfxHit;
    uint16 sfxWall;
    uint16 sfxElectrify;
    uint16 sfxExplosion;
    uint16 sfxAlarm;
};

// Entity Class
struct EntityEggPistonsMKII {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    uint8 type;
    int32 timer;
    int32 pistonID;
    EntityEggPistonsMKII *parent;
    Animator mainAnimator;
    Animator altAnimator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectEggPistonsMKII *EggPistonsMKII;

// Standard Entity Events
void EggPistonsMKII_Update(void);
void EggPistonsMKII_LateUpdate(void);
void EggPistonsMKII_StaticUpdate(void);
void EggPistonsMKII_Draw(void);
void EggPistonsMKII_Create(void *data);
void EggPistonsMKII_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void EggPistonsMKII_EditorDraw(void);
void EggPistonsMKII_EditorLoad(void);
#endif
void EggPistonsMKII_Serialize(void);

// Extra Entity Functions
void EggPistonsMKII_CheckPlayerCollisions_Piston(void);
void EggPistonsMKII_CheckPlayerCollisions_EggPiston(void);
void EggPistonsMKII_CheckPlayerCollisions_Ball(void);
void EggPistonsMKII_CheckPlayerCollisions_Solid(void);

void EggPistonsMKII_Hit(void);
void EggPistonsMKII_Explode(void);

EntityEggPistonsMKII *EggPistonsMKII_GetNextPiston(void);
void EggPistonsMKII_SpawnElecBall(void);
bool32 EggPistonsMKII_CheckPlayerAttacking(EntityPlayer *player);

void EggPistonsMKII_State_SetupArena(void);
void EggPistonsMKII_State_EnterBoss(void);
void EggPistonsMKII_State_PistonReveal(void);
void EggPistonsMKII_State_ClassicMode(void);
void EggPistonsMKII_State_StartPinchMode(void);
void EggPistonsMKII_State_PinchMode(void);
void EggPistonsMKII_State_Destroyed(void);
void EggPistonsMKII_State_Finish(void);

void EggPistonsMKII_StatePiston_Idle(void);
void EggPistonsMKII_StatePiston_Shaking(void);
void EggPistonsMKII_StatePiston_BeginCrushing(void);
void EggPistonsMKII_StatePiston_CrushExtend(void);
void EggPistonsMKII_StatePiston_Retract(void);
void EggPistonsMKII_StatePiston_Explode(void);

void EggPistonsMKII_StateOrbGenerator_Idle(void);
void EggPistonsMKII_StateOrbGenerator_Warning(void);

void EggPistonsMKII_StateOrb_MoveToTargetPos(void);
void EggPistonsMKII_StateOrb_Charging(void);
void EggPistonsMKII_StateOrb_Attacking(void);

void EggPistonsMKII_StateAlarm_Active(void);
void EggPistonsMKII_StateAlarm_Destroyed(void);

void EggPistonsMKII_StateBarrier_Solid(void);
void EggPistonsMKII_StateBarrier_Explode(void);

#endif //! OBJ_EGGPISTONSMKII_H
