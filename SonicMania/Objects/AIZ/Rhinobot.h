#ifndef OBJ_RHINOBOT_H
#define OBJ_RHINOBOT_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectRhinobot {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    uint16 aniFrames;
    uint16 sfxHuff;
};

// Entity Class
struct EntityRhinobot {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDelay);
    int32 unused1;
    int32 timer;
    int32 skidDir;
    int32 moveDir;
    bool32 drawDust;
    int32 acceleration;
    int32 topSpeed;
    Vector2 startPos;
    uint8 startDir;
    Animator bodyAnimator;
    Animator dustAnimator;
};

// Object Struct
extern ObjectRhinobot *Rhinobot;

// Standard Entity Events
void Rhinobot_Update(void);
void Rhinobot_LateUpdate(void);
void Rhinobot_StaticUpdate(void);
void Rhinobot_Draw(void);
void Rhinobot_Create(void *data);
void Rhinobot_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Rhinobot_EditorDraw(void);
void Rhinobot_EditorLoad(void);
#endif
void Rhinobot_Serialize(void);

// Extra Entity Functions
void Rhinobot_DebugSpawn(void);
void Rhinobot_DebugDraw(void);

bool32 Rhinobot_CheckTileCollisions(void);
void Rhinobot_CheckPlayerCollisions(void);
void Rhinobot_CheckOffScreen(void);

void Rhinobot_Delay_Charge(void);
void Rhinobot_Delay_Skidding(void);
void Rhinobot_Delay_SkidFinish(void);

void Rhinobot_State_Init(void);
void Rhinobot_State_Moving(void);
void Rhinobot_State_Skidding(void);
void Rhinobot_State_Idle(void);
void Rhinobot_State_Fall(void);

#endif

#endif //! OBJ_RHINOBOT_H
