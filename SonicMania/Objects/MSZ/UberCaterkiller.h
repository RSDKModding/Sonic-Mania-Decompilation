#ifndef OBJ_UBERCATERKILLER_H
#define OBJ_UBERCATERKILLER_H

#include "Game.h"

#define UBERCATERKILLER_SEGMENT_COUNT (10)

// Object Class
struct ObjectUberCaterkiller {
    RSDK_OBJECT
    Hitbox hitboxSegment;
    bool32 defeated;
    uint16 sfxRocketJet;
    uint16 sfxHit;
    uint16 sfxExplosion2;
    uint16 sfxExplosion3;
    uint16 sfxSandSwim;
    uint16 sfxBumper3;
    uint16 sfxCaterJump;
    uint16 aniFrames;
};

// Entity Class
struct EntityUberCaterkiller {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    Vector2 bodyPositions[UBERCATERKILLER_SEGMENT_COUNT];
    Vector2 bodyVelocity[UBERCATERKILLER_SEGMENT_COUNT];
    int32 bodyTimers[UBERCATERKILLER_SEGMENT_COUNT];
    int32 bodyScales[UBERCATERKILLER_SEGMENT_COUNT];
    int32 bodyInAir[UBERCATERKILLER_SEGMENT_COUNT];
    int32 bodyAngles[UBERCATERKILLER_SEGMENT_COUNT];
    Animator *bodyAnimators[UBERCATERKILLER_SEGMENT_COUNT];
    uint8 aniID;
    uint8 jumpsRemain;
    int32 targetScale;
    int32 targetBodyPos;
    uint8 playerTimers[PLAYER_COUNT];
    Animator headAnimator;
    Animator bodyAnimator;
};

// Object Struct
extern ObjectUberCaterkiller *UberCaterkiller;

// Standard Entity Events
void UberCaterkiller_Update(void);
void UberCaterkiller_LateUpdate(void);
void UberCaterkiller_StaticUpdate(void);
void UberCaterkiller_Draw(void);
void UberCaterkiller_Create(void *data);
void UberCaterkiller_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UberCaterkiller_EditorDraw(void);
void UberCaterkiller_EditorLoad(void);
#endif
void UberCaterkiller_Serialize(void);

// Extra Entity Functions
void UberCaterkiller_DebugSpawn(void);
void UberCaterkiller_DebugDraw(void);

void UberCaterkiller_CheckPlayerCollisions(void);
void UberCaterkiller_Hit(void);
void UberCaterkiller_Explode(void);
void UberCaterkiller_HandleSegmentMoveFX(int32 segmentID);
void UberCaterkiller_SetupBodySegments(int32 x, int32 y);

void UberCaterkiller_State_SetupArena(void);
void UberCaterkiller_PrepareMoveIntoBG(void);

void UberCaterkiller_State_MoveIntoBG(void);
void UberCaterkiller_State_MoveToTargetPos(void);
void UberCaterkiller_State_PrepareHorizontalJump(void);
void UberCaterkiller_State_HorizontalJump(void);
void UberCaterkiller_State_PrepareBGJump(void);
void UberCaterkiller_State_FirstJump(void);
void UberCaterkiller_State_RepeatedJumps(void);
void UberCaterkiller_State_Destroyed(void);
void UberCaterkiller_State_Finish(void);

#endif //! OBJ_UBERCATERKILLER_H
