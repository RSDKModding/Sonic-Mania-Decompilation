#ifndef OBJ_TURBOTURTLE_H
#define OBJ_TURBOTURTLE_H

#include "SonicMania.h"

// Object Class
struct ObjectTurboTurtle {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxWalk;
    uint16 sfxWalk2;
};

// Entity Class
struct EntityTurboTurtle {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    int32 timer;
    Animator animator;
    uint8 currentSide;
    uint16 stepTimer;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    bool32 fanActiveTop;
    bool32 rightFanActive;
    bool32 leftFanActive;
    uint16 stepCount;
    uint8 initialSide;
    bool32 stationary;
    uint8 leftFanDir;
    int32 leftFanDuration;
    int32 leftFanStrength;
    int32 leftFanLength;
    uint8 rightFanDir;
    int32 rightFanDuration;
    int32 rightFanStrength;
    int32 rightFanLength;
};

// Object Struct
extern ObjectTurboTurtle *TurboTurtle;

// Standard Entity Events
void TurboTurtle_Update(void);
void TurboTurtle_LateUpdate(void);
void TurboTurtle_StaticUpdate(void);
void TurboTurtle_Draw(void);
void TurboTurtle_Create(void* data);
void TurboTurtle_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void TurboTurtle_EditorDraw(void);
void TurboTurtle_EditorLoad(void);
#endif
void TurboTurtle_Serialize(void);

// Extra Entity Functions
void TurboTurtle_DebugSpawn(void);
void TurboTurtle_DebugDraw(void);

void TurboTurtle_CheckPlayerCollisions(void);
void TurboTurtle_SetupState(void);
void TurboTurtle_HandleFans(void);
void TurboTurtle_HandleFanDebris(uint8 type, int strength, int length);

bool32 TurboTurtle_FanCheckCB(void);

void TurboTurtle_State_Setup(void);
void TurboTurtle_State_Unknown1(void);
void TurboTurtle_State_Unknown2(void);
void TurboTurtle_State1_Unknown1(void);
void TurboTurtle_State2_Unknown1(void);
void TurboTurtle_State2_Unknown2(void);
void TurboTurtle_State3_Unknown1(void);
void TurboTurtle_State3_Unknown2(void);

#endif //!OBJ_TURBOTURTLE_H
