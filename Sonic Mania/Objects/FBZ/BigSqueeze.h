#ifndef OBJ_BIGSQUEEZE_H
#define OBJ_BIGSQUEEZE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int value1[10], { 3, 12, -0x20000, -0x10000, 13, 0, -0x10000, 14, 0x20000, -0x10000 });
    TABLE(int value2[13], { 4, 8, -0x20000, -0x10000, 9, -0x10000, -0x10000, 10, 0x10000, -0x10000, 11, 0x20000, -0x10000 });
    TABLE(int value3[25], { 8, 0, -0x20000, -0x10000, 1, -0x10000, -0x10000, 2, 0x10000, -0x10000, 3, 0x20000, -0x10000, 4, -0x20000, -0x8000,  5, -0x10000, -0x8000,  6, 0x10000, -0x8000, 7, 0x20000, -0x8000 });
    int value4[4];
    bool32 isRumbling;
    int value6;
    int value7;
    uint16 sfxBossHit;
    uint16 sfxExplosion2;
    uint16 sfxMagnet;
    uint16 sfxOrbinaut;
    uint16 aniFrames;
} ObjectBigSqueeze;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int type;
    int timer2;
    int invincibilityTimer;
    int timer;
    Entity *eggman;
    int field_74;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Hitbox hitbox;
} EntityBigSqueeze;

// Object Struct
extern ObjectBigSqueeze *BigSqueeze;

// Standard Entity Events
void BigSqueeze_Update(void);
void BigSqueeze_LateUpdate(void);
void BigSqueeze_StaticUpdate(void);
void BigSqueeze_Draw(void);
void BigSqueeze_Create(void* data);
void BigSqueeze_StageLoad(void);
void BigSqueeze_EditorDraw(void);
void BigSqueeze_EditorLoad(void);
void BigSqueeze_Serialize(void);

// Extra Entity Functions
bool32 BigSqueeze_RumbleCheckCB(void);

void BigSqueeze_HandleWallCollisions(void);
void BigSqueeze_CheckPlayerCollisions(void);
void BigSqueeze_CheckPlayerCollisions2(void);
void BigSqueeze_Hit(void);

void BigSqueeze_Unknown6(void);
void BigSqueeze_Unknown7(int *debrisData);

void BigSqueeze_StateDraw2_Unknown1(void);
void BigSqueeze_StateDraw3_Unknown1(void);

void BigSqueeze_State1_SetupIntro(void);
void BigSqueeze_State1_SetupEggman(void);
void BigSqueeze_State1_SetupBossArena(void);
void BigSqueeze_State1_SetupBoss(void);

void BigSqueeze_State2_Unknown1(void);
void BigSqueeze_State2_Unknown2(void);
void BigSqueeze_State2_Die(void);
void BigSqueeze_State2_SpawnSignPost(void);
void BigSqueeze_Unknown18(void);

void BigSqueeze_State1_Unknown5(void);

void BigSqueeze_State3_Unknown1(void);
void BigSqueeze_State3_Unknown2(void);

#endif //!OBJ_BIGSQUEEZE_H
