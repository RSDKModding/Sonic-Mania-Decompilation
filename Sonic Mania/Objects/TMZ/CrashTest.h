#ifndef OBJ_CRASHTEST_H
#define OBJ_CRASHTEST_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxExplosion;
    uint16 sfxCrash;
    uint16 sfxTrafficLight;
    uint16 sfxCarRev;
    uint16 sfxSpeedBooster;
} ObjectCrashTest;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 length;
    bool32 debugParticles;
    uint8 activePlayers;
    int32 playerTimers[4];
    Vector2 startPos;
    int32 timer;
    int32 dword84;
    bool32 field_88;
    Vector2 field_8C[8];
    bool32 field_CC;
    int32 field_D0;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox4;
    Hitbox hitbox3;
    Hitbox hitbox5;
    Hitbox hitbox6;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
} EntityCrashTest;

// Object Struct
extern ObjectCrashTest *CrashTest;

// Standard Entity Events
void CrashTest_Update(void);
void CrashTest_LateUpdate(void);
void CrashTest_StaticUpdate(void);
void CrashTest_Draw(void);
void CrashTest_Create(void* data);
void CrashTest_StageLoad(void);
void CrashTest_EditorDraw(void);
void CrashTest_EditorLoad(void);
void CrashTest_Serialize(void);

// Extra Entity Functions
void CrashTest_SetupHitboxes(void);
void CrashTest_CheckOnScreen(void);
void CrashTest_CheckPlayerCrash(void);
void CrashTest_CheckPlayerCollisions(void);
void CrashTest_CheckPlayerRide(void);
void CrashTest_CheckPlayerJump(void);
void CrashTest_Unknown6(void);

void CrashTest_State_Setup(void);
void CrashTest_State_Unknown1(void);
void CrashTest_State_Rev(void);
void CrashTest_State_Move(void);
void CrashTest_State_Unknown4(void);

#endif //!OBJ_CRASHTEST_H
