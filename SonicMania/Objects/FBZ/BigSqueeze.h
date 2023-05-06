#ifndef OBJ_BIGSQUEEZE_H
#define OBJ_BIGSQUEEZE_H

#include "Game.h"

typedef enum {
    BIGSQUEEZE_MANAGER,
    BIGSQUEEZE_BOSS,
    BIGSQUEEZE_CRUSHER_L,
    BIGSQUEEZE_CRUSHER_R,
} BigSqueezeTypes;

// Object Class
struct ObjectBigSqueeze {
    RSDK_OBJECT
    TABLE(int32 prongDebrisInfo[10], { 3, 12, -0x20000, -0x10000, 13, 0, -0x10000, 14, 0x20000, -0x10000 });
    TABLE(int32 domeDebrisInfo[13], { 4, 8, -0x20000, -0x10000, 9, -0x10000, -0x10000, 10, 0x10000, -0x10000, 11, 0x20000, -0x10000 });
    TABLE(int32 baseDebrisInfo[25], { 8,       0, -0x20000, -0x10000, 1, -0x10000, -0x10000, 2, 0x10000, -0x10000, 3, 0x20000, -0x10000, 4, -0x20000,
                                      -0x8000, 5, -0x10000, -0x8000,  6, 0x10000,  -0x8000,  7, 0x20000, -0x8000 });
    int32 crusherX[4];
    bool32 isCrushing;
    int32 crushTimer;
    int32 boundsB;
    uint16 sfxBossHit;
    uint16 sfxExplosion2;
    uint16 sfxMagnet;
    uint16 sfxOrbinaut;
    uint16 aniFrames;
};

// Entity Class
struct EntityBigSqueeze {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    BigSqueezeTypes type;
    int32 setupTimer;
    int32 invincibilityTimer;
    int32 timer;
    EntityEggman *eggman;
    bool32 invincible;
    Animator animator;
    Animator domeAnimator;
    Animator prongsAnimator;
    Animator wheelAnimator;
    Animator electricAnimator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectBigSqueeze *BigSqueeze;

// Standard Entity Events
void BigSqueeze_Update(void);
void BigSqueeze_LateUpdate(void);
void BigSqueeze_StaticUpdate(void);
void BigSqueeze_Draw(void);
void BigSqueeze_Create(void *data);
void BigSqueeze_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BigSqueeze_EditorDraw(void);
void BigSqueeze_EditorLoad(void);
#endif
void BigSqueeze_Serialize(void);

// Extra Entity Functions
bool32 BigSqueeze_SfxCheck_Rumble(void);

void BigSqueeze_HandleWallCollisions(void);
void BigSqueeze_CheckPlayerCollisions_Vulnerable(void);
void BigSqueeze_CheckPlayerCollisions_Electrified(void);
void BigSqueeze_Hit(void);
void BigSqueeze_Explode(void);

void BigSqueeze_HandleBossMovement(void);
void BigSqueeze_SpawnDebris(int32 *debrisInfo);

void BigSqueeze_Draw_Boss(void);
void BigSqueeze_Draw_Crusher(void);

void BigSqueeze_StateManager_SetupIntro(void);
void BigSqueeze_StateManager_SetupEggman(void);
void BigSqueeze_StateManager_SetupArena(void);
void BigSqueeze_StateManager_SetupBoss(void);

void BigSqueeze_StateBoss_Idle(void);
void BigSqueeze_StateBoss_Electrified(void);
void BigSqueeze_StateBoss_Destroyed(void);
void BigSqueeze_StateBoss_DropSignPost(void);
void BigSqueeze_StateManager_HandleOutro(void);

void BigSqueeze_StateManager_HandleBoss(void);

void BigSqueeze_StateCrusher_BeginCrushing(void);
void BigSqueeze_StateCrusher_Crushing(void);

#endif //! OBJ_BIGSQUEEZE_H
