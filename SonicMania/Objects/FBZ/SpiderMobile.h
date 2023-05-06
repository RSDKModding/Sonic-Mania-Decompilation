#ifndef OBJ_SPIDERMOBILE_H
#define OBJ_SPIDERMOBILE_H

#include "Game.h"

typedef enum {
    SPIDERMOBILE_BOSS,
    SPIDERMOBILE_ORB,
    SPIDERMOBILE_BUMPERDEBRIS,
    SPIDERMOBILE_EGGMAN,
} SpiderMobileTypes;

// Object Class
struct ObjectSpiderMobile {
    RSDK_OBJECT
    Hitbox hitboxBumper;
    Hitbox hitboxSpikes;
    Hitbox hitboxUnused;
    Hitbox hitboxOrb;
    Hitbox hitboxPlatform;
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
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
    int32 unused3;
};

// Entity Class
struct EntitySpiderMobile {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 origin;
    EntitySpiderMobile *parent;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 bounceCount;
    int32 bounceDelay;
    int32 knockbackCooldown;
    int32 armMoveAmplitude;
    int32 offsetY;
    int32 angleVel;
    int32 headRotation;
    int32 pincerRotation;
    int32 webCurveAngle;
    int32 moveAmplitude;
    int32 webCurveDirection;
    int32 legAngles[12];
    int32 unused[12];
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
};

// Object Struct
extern ObjectSpiderMobile *SpiderMobile;

// Standard Entity Events
void SpiderMobile_Update(void);
void SpiderMobile_LateUpdate(void);
void SpiderMobile_StaticUpdate(void);
void SpiderMobile_Draw(void);
void SpiderMobile_Create(void *data);
void SpiderMobile_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SpiderMobile_EditorDraw(void);
void SpiderMobile_EditorLoad(void);
#endif
void SpiderMobile_Serialize(void);

// Extra Entity Functions
void SpiderMobile_HandleFallingMovement(void);
void SpiderMobile_HandleRisingMovement(void);
void SpiderMobile_CheckPlayerCollisions(void);
void SpiderMobile_CheckSpikeHit(void);
void SpiderMobile_Hit(void);
void SpiderMobile_Explode(void);
void SpiderMobile_HandlePlatformMovement(void);
void SpiderMobile_HandleWebClimbArmMovement(void);
void SpiderMobile_HandleDestroyedArmMovement(void);
void SpiderMobile_HandleIdleArmMovement(void);
void SpiderMobile_UpdateLimbPositions(void);

void SpiderMobile_Draw_Body(void);
void SpiderMobile_Draw_Bumper(void);
void SpiderMobile_Draw_Cockpit(void);

void SpiderMobile_StateBody_AwaitPlayer(void);
void SpiderMobile_StateBody_SetupArena(void);
void SpiderMobile_StateBody_InitialDrop(void);
void SpiderMobile_StateBody_Rise(void);
void SpiderMobile_StateBody_Climb(void);
void SpiderMobile_StateBody_HandleOrbAttack(void);
void SpiderMobile_StateBody_Destroyed(void);

void SpiderMobile_StateBody_CockpitExplode(void);
void SpiderMobile_StateBody_MovePlatformToEnd(void);
void SpiderMobile_StateBody_FinishedMovingPlatform(void);

void SpiderMobile_State_Bumper(void);

void SpiderMobile_State_Eggman(void);

void SpiderMobile_StateOrb_Charge(void);
void SpiderMobile_StateOrb_Fired(void);

bool32 SpiderMobile_SfxCheck_Elevator(void);

#endif //! OBJ_SPIDERMOBILE_H
