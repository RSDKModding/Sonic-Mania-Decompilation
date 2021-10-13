#ifndef OBJ_SPIDERMOBILE_H
#define OBJ_SPIDERMOBILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitboxBumper;
    Hitbox hitboxSpikes;
    int32 field_14;
    int32 field_18;
    Hitbox hitbox3;
    Hitbox hitboxPlatform;
    int32 boundL;
    int32 boundR;
    int32 startX;
    int32 boundT;
    int32 boundB;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxBumper;
    uint16 sfxCharge;
    uint16 sfxElecBall;
    uint16 sfxFall;
    uint16 sfxFlailing;
    uint16 sfxHitGround;
    uint16 sfxRecovery;
    uint16 sfxElevator;
    uint16 sfxHullClose;
    uint16 aniFrames;
    int32 field_58;
} ObjectSpiderMobile;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 origin;
    Entity *parent;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    int32 field_88;
    int32 field_8C;
    int32 field_90;
    int32 field_94;
    int32 field_98;
    int32 field_9C;
    int32 field_A0;
    int32 legAngles[12];
    int32 field_D4[6];
    int32 field_EC[6];
    Vector2 headPos;
    Vector2 legJointPos[2];
    Vector2 pincer1StartPos;
    Vector2 pincer1EndPos;
    Vector2 pincer2StartPos;
    Vector2 pincer2EndPos;
    Vector2 legPositions[12];
    bool32 groundedStore;
    Animator partAnimator;
    Animator bumperAnimator;
    Animator starAnimator;
    Animator headAnimator;
    Animator orbAnimator;
    Animator legsAnimator;
    Animator pincerAnimator;
    Animator webAnimator;
    Animator eggmanAnimator;
    Animator electricAnimator;
} EntitySpiderMobile;

// Object Struct
extern ObjectSpiderMobile *SpiderMobile;

// Standard Entity Events
void SpiderMobile_Update(void);
void SpiderMobile_LateUpdate(void);
void SpiderMobile_StaticUpdate(void);
void SpiderMobile_Draw(void);
void SpiderMobile_Create(void* data);
void SpiderMobile_StageLoad(void);
void SpiderMobile_EditorDraw(void);
void SpiderMobile_EditorLoad(void);
void SpiderMobile_Serialize(void);

// Extra Entity Functions
void SpiderMobile_Unknown1(void);
void SpiderMobile_Unknown2(void);
void SpiderMobile_CheckPlayerCollisions(void);
void SpiderMobile_CheckSpikeHit(void);
void SpiderMobile_Hit(void);
void SpiderMobile_HandlePlatformMovement(void);
void SpiderMobile_Unknown7(void);
void SpiderMobile_Unknown8(void);
void SpiderMobile_Unknown9(void);
void SpiderMobile_Unknown10(void);

void SpiderMobile_StateDraw_Body(void);
void SpiderMobile_StateDraw2_Unknown1(void);
void SpiderMobile_StateDraw_Eggman(void);

void SpiderMobile_StateBody_SetupBounds(void);
void SpiderMobile_StateBody_SetupArena(void);
void SpiderMobile_StateBody_Unknown1(void);
void SpiderMobile_StateBody_Unknown2(void);
void SpiderMobile_StateBody_Unknown3(void);
void SpiderMobile_StateBody_Unknown4(void);
void SpiderMobile_StateBody_Destroyed(void);

void SpiderMobile_StateEggman_Unknown1(void);
void SpiderMobile_StateEggman_Unknown2(void);
void SpiderMobile_StateEggman_Unknown3(void);

void SpiderMobile_State2_Unknown1(void);

void SpiderMobile_State3_Unknown1(void);

void SpiderMobile_StateOrb_Unknown1(void);
void SpiderMobile_StateOrb_Unknown2(void);

bool32 SpiderMobile_CheckCB(void);

#endif //!OBJ_SPIDERMOBILE_H
