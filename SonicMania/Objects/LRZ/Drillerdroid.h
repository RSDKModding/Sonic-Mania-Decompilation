#ifndef OBJ_DRILLERDROID_H
#define OBJ_DRILLERDROID_H

#include "Game.h"

typedef enum {
    DRILLERDROID_MAIN,
    DRILLERDROID_TARGET,
    DRILLERDROID_UNUSED,
} DrillerdroidTypes;

// Object Class
struct ObjectDrillerdroid {
    RSDK_OBJECT
    Hitbox hitboxCore;
    Hitbox hitboxPistonL;
    Hitbox hitboxPistonR;
    Hitbox hitboxDrillL;
    Hitbox hitboxDrillR;
    int32 pistonPos[2];
    int32 drillPos[2];
    int32 pistonDelay[2];
    int32 drillDelay[2];
    int32 pistonMoveDir[2];
    int32 drillMoveDir[2];
    uint8 platformActive[5];
    uint8 arenaSegment;
    uint8 playerTimers[PLAYER_COUNT];
    uint8 drillSfxTimer;
    bool32 playingDrillSfx;
    uint8 armorHealth;
    uint8 jumpsRemaining;
    uint8 canBreakSegment;
    int32 stalatiteOffset;
    EntityDrillerdroid *boss;
    uint16 aniFrames;
    uint16 ticFrames;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxDrill;
    uint16 sfxImpact;
    uint16 sfxJump;
    uint16 sfxTargeting;
    uint16 sfxSizzle;
    uint16 sfxDrop;
    uint16 sfxFail;
    uint16 sfxClang;
};

// Entity Class
struct EntityDrillerdroid {
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
extern ObjectDrillerdroid *Drillerdroid;

// Standard Entity Events
void Drillerdroid_Update(void);
void Drillerdroid_LateUpdate(void);
void Drillerdroid_StaticUpdate(void);
void Drillerdroid_Draw(void);
void Drillerdroid_Create(void *data);
void Drillerdroid_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Drillerdroid_EditorDraw(void);
void Drillerdroid_EditorLoad(void);
#endif
void Drillerdroid_Serialize(void);

// Extra Entity Functions
void Drillerdroid_Hit(void);
void Drillerdroid_CheckPlayerCollisions(void);
void Drillerdroid_Explode(void);
void Drillerdroid_SpawnDebris(int32 offset);

void Drillerdroid_State_SetupArena(void);
void Drillerdroid_State_AwaitPlayer(void);
void Drillerdroid_State_Dropping(void);
void Drillerdroid_State_Landed(void);
void Drillerdroid_State_LandRecoil(void);
void Drillerdroid_State_DecideNextMove(void);
void Drillerdroid_State_PrepareJump(void);
void Drillerdroid_State_Jumping(void);
void Drillerdroid_State_Drilling(void);
void Drillerdroid_State_FinishDrilling(void);
void Drillerdroid_State_Overheat(void);
void Drillerdroid_State_OverheatRecoil(void);
void Drillerdroid_State_JumpTargetDelay(void);
void Drillerdroid_State_PrepareJumpTarget(void);
void Drillerdroid_State_JumpTargeting(void);
void Drillerdroid_State_DecidingDropPos(void);
void Drillerdroid_State_DropFailReset(void);
void Drillerdroid_State_OverheatRecoil_DestroyedSegment(void);
void Drillerdroid_State_Cooldown(void);
void Drillerdroid_State_ResetFromCooldown(void);
void Drillerdroid_State_Destroyed(void);
void Drillerdroid_State_Finish(void);
void Drillerdroid_State_DropSignPost(void);

void Drillerdroid_Draw_Boss(void);

void Drillerdroid_Draw_Simple(void);

void Drillerdroid_State_Target(void);
void Drillerdroid_Draw_Target(void);

#endif //! OBJ_DRILLERDROID_H
