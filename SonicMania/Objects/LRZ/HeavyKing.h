#ifndef OBJ_HEAVYKING_H
#define OBJ_HEAVYKING_H

#include "Game.h"

// Object Class
struct ObjectHeavyKing {
    RSDK_OBJECT
    TABLE(int32 attackPattern[32], { 0, 0, 1, 0, 2, 2, 1, 0, 0, 1, 1, 2, 0, 2, 0, 0, 2, 1, 1, 2, 1, 0, 1, 0, 2, 2, 0, 1, 0, 0, 2, 1 });
    int32 attackPatternPos;
    Hitbox hitboxBody;
    Hitbox hitboxBoss;
    Hitbox hitboxCharging;
    int32 boundsL;
    int32 boundsM;
    int32 boundsR;
    int32 spinRaySpawnPos;
    int32 startY;
    uint16 sfxHit;
    uint16 sfxImpact2;
    uint16 sfxImpact5;
    uint16 sfxCharge;
    uint16 sfxExplosion;
    uint16 sfxFreeze;
    uint16 sfxRodPlant;
    uint16 sfxRodShine;
    uint16 sfxElecOn;
    uint16 sfxTwinCharge;
    uint16 sfxImpact6;
    uint16 aniFrames;
    uint16 cutsceneFrames;
};

// Entity Class
struct EntityHeavyKing {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateStore);
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 curAttack;
    int32 attacksRemaining;
    Vector2 originPos;
    EntityKingClaw *claw;
    EntityHPZEmerald *masterEmerald;
    EntityHPZEmerald *targetEmerald;
    Animator bodyAnimator;
    Animator scepterAnimator;
    Animator electricityAnimator;
    Animator storedBodyAnimator;
    Animator storedScepterAnimator;
};

// Object Struct
extern ObjectHeavyKing *HeavyKing;

// Standard Entity Events
void HeavyKing_Update(void);
void HeavyKing_LateUpdate(void);
void HeavyKing_StaticUpdate(void);
void HeavyKing_Draw(void);
void HeavyKing_Create(void *data);
void HeavyKing_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HeavyKing_EditorDraw(void);
void HeavyKing_EditorLoad(void);
#endif
void HeavyKing_Serialize(void);

// Extra Entity Functions
void HeavyKing_CheckPlayerCollisions(void);
void HeavyKing_CheckPlayerCollisions_Charging(void);
void HeavyKing_Hit(void);
void HeavyKing_HandleClawMovement(void);
void HeavyKing_HandleAnimators(void);
void HeavyKing_HandleHoverMovement(void);
void HeavyKing_FindTargetEmerald(void);

void HeavyKing_StartLeap(int32 startFrame);
void HeavyKing_HandleAttackFinish(void);
void HeavyKing_CreateSpinRayFX(void);
void HeavyKing_CreateExpandRingFX(void);
void HeavyKing_CreateLaser(void);
void HeavyKing_CreateExpandRing(void);

// Cutscene States
void HeavyKing_State_SetupArena(void);
void HeavyKing_State_HandleCutsceneSetup(void);
void HeavyKing_StateCutscene_PlayerLookUp(void);
void HeavyKing_StateCutscene_EnterKing(void);
void HeavyKing_StateCutscene_ReturnCamToPlayer(void);
void HeavyKing_StateCutscene_GrabMasterEmerald(void);
void HeavyKing_StateCutscene_FinishThinking(void);
void HeavyKing_StateCutscene_GetHigherGround(void);
void HeavyKing_StateCutscene_Complaining(void);
void HeavyKing_StateCutscene_ChargeSpindash(void);
void HeavyKing_StateCutscene_AttackClaw(void);
void HeavyKing_StateCutscene_AttackRebound(void);

// Boss States
void HeavyKing_State_JumpToTargetEmerald(void);
void HeavyKing_State_Leaping(void);
void HeavyKing_State_LeapToMasterEmerald(void);
void HeavyKing_State_LandedOnMasterEmerald(void);
void HeavyKing_State_ChargeStart(void);
void HeavyKing_State_Charging(void);
void HeavyKing_State_ChargeStop(void);
void HeavyKing_State_PrepareHover(void);
void HeavyKing_State_StartHovering(void);
void HeavyKing_State_Hovering(void);
void HeavyKing_State_LaserAttack(void);
void HeavyKing_State_ExpandRingAttack(void);
void HeavyKing_State_TwinChargeAttack(void);
void HeavyKing_State_HitRecoil(void);
void HeavyKing_State_Destroyed(void);
void HeavyKing_State_Escape(void);
void HeavyKing_State_Finish(void);

#endif //! OBJ_HEAVYKING_H
