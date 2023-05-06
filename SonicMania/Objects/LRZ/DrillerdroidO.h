#ifndef OBJ_DRILLERDROIDO_H
#define OBJ_DRILLERDROIDO_H

#include "Game.h"

typedef enum {
    DRILLERDROIDO_MAIN,
    DRILLERDROIDO_FIREBALLEMITTER,
    DRILLERDROIDO_TARGET,
    DRILLERDROIDO_UNUSED1,
    DRILLERDROIDO_UNUSED2,
} DrillerdroidOTypes;

// Object Class
struct ObjectDrillerdroidO {
    RSDK_OBJECT
    Hitbox hitboxCore;
    Hitbox hitboxPistonL;
    Hitbox hitboxPistonR;
    Hitbox hitboxDrillL;
    Hitbox hitboxDrillR;
    Hitbox hitboxLandPosSegmentBreak;
    Hitbox hitboxLandPosR;
    Hitbox hitboxLandPosL;
    Hitbox hitboxEmitterRange;
    int32 pistonPos[2];
    int32 drillPos[2];
    int32 pistonDelay[2];
    int32 drillDelay[2];
    int32 pistonMoveDir[2];
    int32 drillMoveDir[2];
    uint8 drillSfxTimer;
    bool32 playingDrillSfx;
    uint8 canBreakSegment;
    int32 childSlotStart;
    uint8 currentLevel;
    bool32 emitFireballs;
    EntityDrillerdroidO *boss;
    uint16 aniFrames;
    uint16 ticFrames;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxDrill;
    uint16 sfxImpact;
    uint16 sfxJump;
    uint16 sfxTargeting;
    uint16 sfxSizzle;
};

// Entity Class
struct EntityDrillerdroidO {
    RSDK_ENTITY
    uint8 type;
    StateMachine(state);
    StateMachine(stateDraw);
    Animator mainAnimator;
    Animator armorAnimator;
    Animator targetLockAnimator;
    EntityPlayer *target;
    Vector2 targetEdgeOffset;
    int32 startY;
    int32 timer;
    uint8 invincibilityTimer;
    int8 health;
};

// Object Struct
extern ObjectDrillerdroidO *DrillerdroidO;

// Standard Entity Events
void DrillerdroidO_Update(void);
void DrillerdroidO_LateUpdate(void);
void DrillerdroidO_StaticUpdate(void);
void DrillerdroidO_Draw(void);
void DrillerdroidO_Create(void *data);
void DrillerdroidO_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void DrillerdroidO_EditorDraw(void);
void DrillerdroidO_EditorLoad(void);
#endif
void DrillerdroidO_Serialize(void);

// Extra Entity Functions
void DrillerdroidO_CheckPlayerCollisions(void);
void DrillerdroidO_Explode(void);
void DrillerdroidO_SpawnDebris(int32 offset);

void DrillerdroidO_State_SetupArena(void);
void DrillerdroidO_State_AwaitPlayer(void);
void DrillerdroidO_State_Dropping(void);
void DrillerdroidO_State_Landed(void);
void DrillerdroidO_State_LandRecoil(void);
void DrillerdroidO_State_BeginDrilling(void);
void DrillerdroidO_State_Drilling(void);
void DrillerdroidO_State_FinishDrilling(void);
void DrillerdroidO_State_Overheat(void);
void DrillerdroidO_State_OverheatRecoil(void);
void DrillerdroidO_State_JumpTargetDelay(void);
void DrillerdroidO_State_PrepareJumpTarget(void);
void DrillerdroidO_State_JumpTargeting(void);
void DrillerdroidO_State_DestroyRockPiles(void);
void DrillerdroidO_State_NextLevelDelay(void);
void DrillerdroidO_State_MoveToNextLevel(void);
void DrillerdroidO_State_DecidingDropPos(void);
void DrillerdroidO_State_OverheatRecoil_DestroyedSegment(void);
void DrillerdroidO_State_Cooldown(void);
void DrillerdroidO_State_ResetFromCooldown(void);
void DrillerdroidO_State_Destroyed(void);
void DrillerdroidO_State_Finish(void);

void DrillerdroidO_Draw_Boss(void);
void DrillerdroidO_Draw_Simple(void);

void DrillerdroidO_State_Target(void);
void DrillerdroidO_Draw_Target(void);

void DrillerdroidO_State_FireballEmitter(void);
void DrillerdroidO_Draw_FireballEmitter(void);

#endif //! OBJ_DRILLERDROIDO_H
