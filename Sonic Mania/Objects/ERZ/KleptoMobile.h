#ifndef OBJ_KLEPTOMOBILE_H
#define OBJ_KLEPTOMOBILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 debrisInfo[29], { 4,        0, 0, -0x20000, -0x28000, 1, 0, -0x20000, -0x28000, 2, 0, 0x20000, -0x20000, 3, 0, 0x28000,
                                  -0x20000, 4, 0, -0x10000, -0x10000, 5, 0, 0x10000,  -0x10000, 6, 0, -0x8000, -0x10000 });
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxFlail;
    uint16 sfxWhack;
    uint16 sfxPowerUp;
    uint16 sfxRocketJet;
    uint16 aniFrames;
    bool32 defeated;
} ObjectKleptoMobile;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    Vector2 field_70;
    int32 frameIDs[10];
    Vector2 framePositions[10];
    Vector2 field_F0;
    Vector2 field_F8;
    Vector2 field_100;
    int32 field_108;
    int32 field_10C;
    int32 field_110;
    bool32 field_114;
    Vector2 circlePos;
    bool32 field_120;
    int32 angle2;
    int32 circleRadius;
    int32 explosionVolume;
    Entity *parent;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
    Animator animator7;
    Animator animator8;
    Animator animator9;
    Animator animator10;
    Animator animator11;
    Hitbox hitbox;
} EntityKleptoMobile;

// Object Struct
extern ObjectKleptoMobile *KleptoMobile;

// Standard Entity Events
void KleptoMobile_Update(void);
void KleptoMobile_LateUpdate(void);
void KleptoMobile_StaticUpdate(void);
void KleptoMobile_Draw(void);
void KleptoMobile_Create(void* data);
void KleptoMobile_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void KleptoMobile_EditorDraw(void);
void KleptoMobile_EditorLoad(void);
#endif
void KleptoMobile_Serialize(void);

// Extra Entity Functions
void KleptoMobile_HandleAnimations(void);
void KleptoMobile_CheckPlayerCollisions(void);
void KleptoMobile_Hit(void);
void KleptoMobile_HandleFrames(void);
void KleptoMobile_SwitchToKing(void);

void KleptoMobile_StateDraw_Unknown1(void);
void KleptoMobile_State_SetupArena(void);
void KleptoMobile_State_Unknown1(void);
void KleptoMobile_State_Unknown2(void);
void KleptoMobile_State_Unknown3(void);
void KleptoMobile_HandleArmPositions(void);
void KleptoMobile_State_Unknown4(void);
void KleptoMobile_State_Unknown5(void);
void KleptoMobile_State_Unknown6(void);
void KleptoMobile_State_Switch(void);
void KleptoMobile_State_HitFall(void);

void KleptoMobile_State3_Unknown1(void);
void KleptoMobile_State3_Unknown2(void);
void KleptoMobile_StateDraw3_Unknown1(void);

void KleptoMobile_CheckPlayerCollisions_Arm(void);

void KleptoMobile_State1_Unknown1(void);
void KleptoMobile_State1_Unknown2(void);
void KleptoMobile_State1_Unknown3(void);
void KleptoMobile_State1_Unknown4(void);
void KleptoMobile_StateDraw1_Unknown1(void);

void KleptoMobile_State_Destroyed(void);
void KleptoMobile_State_Explode(void);
void KleptoMobile_State_CutsceneExplode(void);

#endif //!OBJ_KLEPTOMOBILE_H
