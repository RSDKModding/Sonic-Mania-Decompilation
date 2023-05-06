#ifndef OBJ_TORNADO_H
#define OBJ_TORNADO_H

#include "Game.h"

typedef enum {
    TORNADO_MODE_IDLE,
    TORNADO_MODE_JUMP_RECOIL,
    TORNADO_MODE_JUMP_REBOUND,
    TORNADO_MODE_LAND,
} TornadoCollideModes;

// Object Class
struct ObjectTornado {
    RSDK_OBJECT
    TABLE(int32 flameOffsets[5], { -0x20000, -0x10000, 0, 0x10000, 0x20000 });
    uint16 aniFrames;
    uint16 knuxFrames;
    uint16 sfxExplosion;
    uint16 sfxImpact;
};

// Entity Class
struct EntityTornado {
    RSDK_ENTITY
    StateMachine(state);
    int32 collideTimer;
    int32 timer;
    int32 turnAngle;
    int32 mode;
    bool32 isStood;
    bool32 showFlame;
    int32 storeY;
    int32 gravityForce;
    int32 prevPosY;
    int32 moveVelocityY;
    Vector2 movePos;
    Vector2 knuxPos;
    Vector2 knuxVel;
    int32 offsetX;
    Animator animatorTornado;
    Animator animatorPropeller;
    Animator animatorPilot;
    Animator animatorFlame;
    Animator animatorKnux;
};

// Object Struct
extern ObjectTornado *Tornado;

// Standard Entity Events
void Tornado_Update(void);
void Tornado_LateUpdate(void);
void Tornado_StaticUpdate(void);
void Tornado_Draw(void);
void Tornado_Create(void *data);
void Tornado_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Tornado_EditorDraw(void);
void Tornado_EditorLoad(void);
#endif
void Tornado_Serialize(void);

// Extra Entity Functions
void Tornado_State_Init(void);
void Tornado_State_SetupMSZ1Intro(void);
void Tornado_State_MSZ1Intro(void);
void Tornado_State_KnuxKnockedOff(void);
void Tornado_HandlePlayerCollisions(void);
void Tornado_State_PlayerControlled(void);
void Tornado_State_Mayday(void);
void Tornado_State_FlyAway_Right(void);
void Tornado_State_FlyAway_Left(void);

#endif //! OBJ_TORNADO_H
