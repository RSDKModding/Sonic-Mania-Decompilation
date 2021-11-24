#ifndef OBJ_BIGSQUEEZE_H
#define OBJ_BIGSQUEEZE_H

#include "SonicMania.h"

typedef enum {
    BIGSQUEEZE_MANAGER,
    BIGSQUEEZE_BOSS,
    BIGSQUEEZE_CRUSHER_L,
    BIGSQUEEZE_CRUSHER_R,
}BigSqueezeTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 prongDebrisInfo[10], { 3, 12, -0x20000, -0x10000, 13, 0, -0x10000, 14, 0x20000, -0x10000 });
    TABLE(int32 domeDebrisInfo[13], { 4, 8, -0x20000, -0x10000, 9, -0x10000, -0x10000, 10, 0x10000, -0x10000, 11, 0x20000, -0x10000 });
    TABLE(int32 baseDebrisInfo[25], { 8, 0, -0x20000, -0x10000, 1, -0x10000, -0x10000, 2, 0x10000, -0x10000, 3, 0x20000, -0x10000, 4, -0x20000, -0x8000,  5, -0x10000, -0x8000,  6, 0x10000, -0x8000, 7, 0x20000, -0x8000 });
    int32 value4[4];
    bool32 isRumbling;
    int32 value6;
    int32 value7;
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
    BigSqueezeTypes type;
    int32 setupTimer;
    int32 invincibilityTimer;
    int32 timer;
    Entity *eggman;
    bool32 invincible;
    Animator animator;
    Animator domeAnimator;
    Animator prongsAnimator;
    Animator wheelAnimator;
    Animator electricAnimator;
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
#if RETRO_INCLUDE_EDITOR
void BigSqueeze_EditorDraw(void);
void BigSqueeze_EditorLoad(void);
#endif
void BigSqueeze_Serialize(void);

// Extra Entity Functions
bool32 BigSqueeze_RumbleCheckCB(void);

void BigSqueeze_HandleWallCollisions(void);
void BigSqueeze_CheckPlayerCollisions(void);
void BigSqueeze_CheckPlayerCollisions2(void);
void BigSqueeze_Hit(void);

void BigSqueeze_Unknown6(void);
void BigSqueeze_SpawnDebris(int32 *debrisData);

void BigSqueeze_Draw_Boss(void);
void BigSqueeze_Draw_Crusher(void);

void BigSqueeze_State1_SetupIntro(void);
void BigSqueeze_State1_SetupEggman(void);
void BigSqueeze_State1_SetupArena(void);
void BigSqueeze_State1_SetupBoss(void);

void BigSqueeze_State2_Unknown1(void);
void BigSqueeze_State2_Unknown2(void);
void BigSqueeze_State2_Die(void);
void BigSqueeze_State2_SpawnSignPost(void);
void BigSqueeze_StateManager_Outro(void);

void BigSqueeze_State1_Unknown5(void);

void BigSqueeze_StateCrusher_BeginCrushing(void);
void BigSqueeze_StateCrusher_Crushing(void);

#endif //!OBJ_BIGSQUEEZE_H
