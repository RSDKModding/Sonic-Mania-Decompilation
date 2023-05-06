#ifndef OBJ_CRASHTEST_H
#define OBJ_CRASHTEST_H

#include "Game.h"

// Object Class
struct ObjectCrashTest {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxExplosion;
    uint16 sfxCrash;
    uint16 sfxTrafficLight;
    uint16 sfxCarRev;
    uint16 sfxSpeedBooster;
};

// Entity Class
struct EntityCrashTest {
    RSDK_ENTITY
    StateMachine(state);
    int32 length;
    bool32 debugParticles;
    uint8 activePlayers;
    int32 playerTimers[PLAYER_COUNT];
    Vector2 startPos;
    int32 timer;
    int32 travelDistance;
    bool32 boosterActive;
    Vector2 debrisOffsets[8];
    bool32 turningGreen;
    bool32 prevTurningGreen;
    Hitbox hitboxSeat;
    Hitbox hitboxFront;
    Hitbox hitboxBack;
    Hitbox hitboxFloor;
    Hitbox hitboxStopper;
    Hitbox hitboxBooster;
    Animator carAnimator;
    Animator driverAnimator;
    Animator lightAnimator;
    Animator stopperAnimator;
};

// Object Struct
extern ObjectCrashTest *CrashTest;

// Standard Entity Events
void CrashTest_Update(void);
void CrashTest_LateUpdate(void);
void CrashTest_StaticUpdate(void);
void CrashTest_Draw(void);
void CrashTest_Create(void *data);
void CrashTest_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CrashTest_EditorDraw(void);
void CrashTest_EditorLoad(void);
#endif
void CrashTest_Serialize(void);

// Extra Entity Functions
void CrashTest_SetupHitboxes(void);
void CrashTest_CheckOffScreen(void);
void CrashTest_HandlePlayerCrash(void);
void CrashTest_CheckPlayerCollisionsSolid(void);
void CrashTest_CheckPlayerRide(void);
void CrashTest_CheckPlayerJump(void);
void CrashTest_HandleLightAnims(void);

void CrashTest_State_Init(void);
void CrashTest_State_AwaitPlayer(void);
void CrashTest_State_Rev(void);
void CrashTest_State_Move(void);
void CrashTest_State_Crashed(void);

#endif //! OBJ_CRASHTEST_H
