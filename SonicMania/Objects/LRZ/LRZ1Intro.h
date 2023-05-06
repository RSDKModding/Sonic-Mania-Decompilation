#ifndef OBJ_LRZ1INTRO_H
#define OBJ_LRZ1INTRO_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectLRZ1Intro {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxSubLand;
    uint16 sfxWalkerLegs2;
    uint16 sfxLava;
};

// Entity Class
struct EntityLRZ1Intro {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 originY;
    int32 spawnPosY;
    Vector2 playerOffset[PLAYER_COUNT];
    Animator animator;
};

// Object Struct
extern ObjectLRZ1Intro *LRZ1Intro;

// Standard Entity Events
void LRZ1Intro_Update(void);
void LRZ1Intro_LateUpdate(void);
void LRZ1Intro_StaticUpdate(void);
void LRZ1Intro_Draw(void);
void LRZ1Intro_Create(void *data);
void LRZ1Intro_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LRZ1Intro_EditorDraw(void);
void LRZ1Intro_EditorLoad(void);
#endif
void LRZ1Intro_Serialize(void);

// Extra Entity Functions
void LRZ1Intro_HandlePlayerCollisions(void);
void LRZ1Intro_HandlePlayerMovement(void);
void LRZ1Intro_HandleExplosions(void);
void LRZ1Intro_HandleExplosions(void);

void LRZ1Intro_State_SetupActors(void);
void LRZ1Intro_State_IntroDelay(void);
void LRZ1Intro_State_EnterSub(void);
void LRZ1Intro_State_RidingSub(void);
void LRZ1Intro_State_CrashedSub(void);
void LRZ1Intro_State_SubSinking(void);

#endif

#endif //! OBJ_LRZ1INTRO_H
