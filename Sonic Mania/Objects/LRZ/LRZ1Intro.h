#ifndef OBJ_LRZ1INTRO_H
#define OBJ_LRZ1INTRO_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxSubLand;
    uint16 sfxWalkerLegs2;
    uint16 sfxLava;
} ObjectLRZ1Intro;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    int originY;
    int spawnPosY;
    Vector2 field_68[PLAYER_MAX];
    Animator animator;
} EntityLRZ1Intro;

// Object Struct
extern ObjectLRZ1Intro *LRZ1Intro;

// Standard Entity Events
void LRZ1Intro_Update(void);
void LRZ1Intro_LateUpdate(void);
void LRZ1Intro_StaticUpdate(void);
void LRZ1Intro_Draw(void);
void LRZ1Intro_Create(void* data);
void LRZ1Intro_StageLoad(void);
void LRZ1Intro_EditorDraw(void);
void LRZ1Intro_EditorLoad(void);
void LRZ1Intro_Serialize(void);

// Extra Entity Functions
void LRZ1Intro_HandlePlayerCollisions(void);
void LRZ1Intro_HandlePlayerMovement(void);
void LRZ1Intro_HandleShipExplosions(void);
void LRZ1Intro_HandleExplosions(void);

void LRZ1Intro_State_Unknown1(void);
void LRZ1Intro_State_Unknown2(void);
void LRZ1Intro_State_Unknown3(void);
void LRZ1Intro_State_Unknown4(void);
void LRZ1Intro_State_Unknown5(void);
void LRZ1Intro_State_Unknown6(void);

#endif

#endif //!OBJ_LRZ1INTRO_H
