#ifndef OBJ_METALSONIC_H
#define OBJ_METALSONIC_H

#include "Game.h"

typedef enum {
    MS_ANI_IDLE,
    MS_ANI_TAUNT,
    MS_ANI_READY,
    MS_ANI_HOVER,
    MS_ANI_HOVERTURN,
    MS_ANI_FLY,
    MS_ANI_DASHATTACK,
    MS_ANI_ELECTRICATTACK,
    MS_ANI_BALLATTACK,
    MS_ANI_ENTERPANEL,
    MS_ANI_DEFEATED,
    MS_ANI_BOOSTER_WEAK,
    MS_ANI_BOOSTER_STRONG,
    MS_ANI_BOOSTER_INTRO,
} MetalSonicAniIDs;

typedef enum {
    MS_ATTACK_DASH,
    MS_ATTACK_ELECTRIC,
    MS_ATTACK_BALL,
} MetalSonicAttackIDs;

// Object Class
struct ObjectMetalSonic {
    RSDK_OBJECT
    int32 unused;
    int32 invincibilityTimerPanel;
    uint16 finishedAttacks;
    uint16 sfxHit;
    uint16 sfxExplosion2;
    uint16 sfxExplosion3;
    uint16 sfxRumble;
    uint16 sfxJump2;
    uint16 sfxSpecialRing;
    uint16 sfxMSElecPulse;
    uint16 sfxMSBall;
    uint16 sfxMSFireball;
    uint16 sfxBeep3;
    uint16 sfxBeep4;
    uint16 sfxRockemSockem;
    uint16 sfxMSShoot;
    uint16 sfxMSChargeFire;
#if MANIA_USE_PLUS
    uint16 sfxMSTransform;
    uint16 sfxTransform2;
#endif
    Hitbox hitboxHover;
    Hitbox hitboxDash;
    uint16 aniFrames;
};

// Entity Class
struct EntityMetalSonic {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 targetPos;
    int32 timer;
    int32 unused1;
    int32 unused2;
    int32 health;
    int32 unused3;
    Vector2 hoverVelocity;
    Vector2 targetVelocity;
    int32 attackTimer;
    uint8 attackType;
    int32 unused4;
    int32 invincibilityTimer;
    EntityMSPanel *panel;
    Vector2 unusedVec1;
    Vector2 unusedVec2;
    Vector2 unusedVec3;
    Hitbox *outerBox;
    Hitbox *innerBox;
    Animator metalSonicAnimator;
    Animator boosterAnimator;
};

// Object Struct
extern ObjectMetalSonic *MetalSonic;

// Standard Entity Events
void MetalSonic_Update(void);
void MetalSonic_LateUpdate(void);
void MetalSonic_StaticUpdate(void);
void MetalSonic_Draw(void);
void MetalSonic_Create(void *data);
void MetalSonic_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MetalSonic_EditorDraw(void);
void MetalSonic_EditorLoad(void);
#endif
void MetalSonic_Serialize(void);

// Extra Entity Functions
void MetalSonic_HandleStageWrap(void);
void MetalSonic_ProcessBGParallax(int32 mult);
void MetalSonic_HandleVelocity(void);
void MetalSonic_HandleAnimDir(void);
#if !MANIA_USE_PLUS
void MetalSonic_CheckPlayerCollisions(void);
void MetalSonic_Hit(void);
void MetalSonic_Explode(void);
#endif

// Phase 1
void MetalSonic_State_SetupArena(void);
void MetalSonic_State_AwaitPlayer(void);
void MetalSonic_State_WaitForHologram(void);
void MetalSonic_State_Appear(void);
void MetalSonic_State_Land(void);
void MetalSonic_State_Taunt(void);
void MetalSonic_State_GetReady(void);
void MetalSonic_State_Ready(void);
void MetalSonic_State_Start(void);
void MetalSonic_State_EnterHoverMode(void);
void MetalSonic_State_Hovering(void);
void MetalSonic_State_PrepareAttack(void);
void MetalSonic_State_StartAttack(void);
void MetalSonic_State_SetupBallAttack_Phase1(void);
void MetalSonic_State_BallAttack_Phase1(void);
void MetalSonic_State_SetupElectricAttack_Phase1(void);
void MetalSonic_State_ElectricAttack_Phase1(void);
void MetalSonic_State_SetupDashAttack_Phase1(void);
void MetalSonic_State_DashAttack_Phase1(void);

// Phase 2
void MetalSonic_State_EnterPanel(void);
void MetalSonic_State_StartPanelSequence(void);
void MetalSonic_HandlePanelAttack(void);
void MetalSonic_State_OpenFactoryDoor(void);
void MetalSonic_State_HandleSilverSonics(void);
void MetalSonic_State_PanelExplosion(void);
void MetalSonic_State_ExitFactory(void);
void MetalSonic_State_PrepareFinalChase(void);

#if MANIA_USE_PLUS // Phase 3 (VS GigaMetal)
void MetalSonic_State_WaitForRuby(void);
void MetalSonic_State_ObtainRuby(void);
void MetalSonic_State_Transform(void);
void MetalSonic_State_Defeated(void);
#else // Phase 3 (VS Metal Sonic)
void MetalSonic_State_SetupSpikeWall(void);
void MetalSonic_State_FlyToSpikeWall(void);
void MetalSonic_State_LandNearSpikeWall(void);
void MetalSonic_State_Taunt_Phase2(void);
void MetalSonic_State_Ready_Phase2(void);
void MetalSonic_State_StartSpikeWallMovement(void);
void MetalSonic_State_AccelerateSpikeWall(void);
void MetalSonic_State_Hover_Phase2(void);
void MetalSonic_State_SetupBallAttack_Phase2(void);
void MetalSonic_State_BallAttack_Phase2(void);
void MetalSonic_State_FinishAttack_Phase2(void);
void MetalSonic_State_SetupElectricAttack_Phase2(void);
void MetalSonic_State_StartElectricAttack_Phase2(void);
void MetalSonic_State_ElectricAttack_Phase2(void);
void MetalSonic_State_DashAttack_Phase2(void);
void MetalSonic_State_Explode(void);
void MetalSonic_State_Defeated(void);
void MetalSonic_State_Finish(void);
void MetalSonic_State_None(void);

void MetalSonic_StateWall_Fall(void);
void MetalSonic_StateWall_Move(void);
#endif

#endif //! OBJ_METALSONIC_H
