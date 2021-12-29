#ifndef OBJ_TORNADO_H
#define OBJ_TORNADO_H

#include "SonicMania.h"

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
    int32 collideMode;
    bool32 isStood;
    bool32 showFlame;
    int32 storeY;
    int32 gravityForce;
    int32 prevPosY;
    int32 moveVelocityY;
    Vector2 movePos;
    Vector2 knuxOffset;
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
void Tornado_Create(void* data);
void Tornado_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Tornado_EditorDraw(void);
void Tornado_EditorLoad(void);
#endif
void Tornado_Serialize(void);

// Extra Entity Functions
void Tornado_State_Setup(void);
void Tornado_State_SetupPlayers(void);
void Tornado_Unknown3(void);
void Tornado_Unknown4(void);
void Tornado_HandlePlayerCollisions(void);
void Tornado_Unknown6(void);
void Tornado_Unknown7(void);
void Tornado_Unknown8(void);
void Tornado_Unknown9(void);

#endif //!OBJ_TORNADO_H
