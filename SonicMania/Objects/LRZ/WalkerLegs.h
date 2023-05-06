#ifndef OBJ_WALKERLEGS_H
#define OBJ_WALKERLEGS_H

#include "Game.h"

// Object Class
struct ObjectWalkerLegs {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 particleFrames;
    uint16 sfxWalkerLegs;
    uint16 sfxWalkerLegs2;
    Animator legAnimator;
    Animator linkAnimator;
    bool32 hasAchievement;
};

// Entity Class
struct EntityWalkerLegs {
    RSDK_ENTITY
    StateMachine(state);
    uint16 steps;
    int32 activePlayers[2];
    Vector2 startPos;
    Vector2 legPos[2];
    Vector2 legCollisionOffset[2];
    Vector2 prevLegPos[2];
    int32 smokeSpawnY[2];
    int32 startAngle;
    int32 activeLeg;
    bool32 finishedStep;
    int32 angleVel;
    uint16 stepCount;
    int32 smokeSpawnTimer[2];
    int32 timer;
};

// Object Struct
extern ObjectWalkerLegs *WalkerLegs;

// Standard Entity Events
void WalkerLegs_Update(void);
void WalkerLegs_LateUpdate(void);
void WalkerLegs_StaticUpdate(void);
void WalkerLegs_Draw(void);
void WalkerLegs_Create(void *data);
void WalkerLegs_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void WalkerLegs_EditorDraw(void);
void WalkerLegs_EditorLoad(void);
#endif
void WalkerLegs_Serialize(void);

// Extra Entity Functions
void WalkerLegs_DrawSprites(void);
void WalkerLegs_CheckOffScreen(void);
void WalkerLegs_HandlePlayerMovement(void);
void WalkerLegs_CheckObjectCrush(void);
void WalkerLegs_CheckStepTrigger(void);
void WalkerLegs_CheckTileCollisions(void);
void WalkerLegs_CheckStoodLava(void);
void WalkerLegs_CreateDebris(bool32 isRightLeg, bool32 isMagma);
void WalkerLegs_CreateSmoke(bool32 isRightLeg);

void WalkerLegs_State_Init(void);
void WalkerLegs_State_Idle(void);
void WalkerLegs_State_Stepping(void);
void WalkerLegs_State_TryToReset(void);

#endif //! OBJ_WALKERLEGS_H
