#ifndef OBJ_PHANTOMHAND_H
#define OBJ_PHANTOMHAND_H

#include "Game.h"

// Object Class
struct ObjectPhantomHand {
    RSDK_OBJECT
    TABLE(int32 debrisInfo[41], { 10,       0, 0, -0x20000, -0x10000, 1, 0, -0x10000, -0x10000, 2, 0, 0x10000, -0x10000, 3, 0, 0x20000,
                                  -0x10000, 4, 0, -0x20000, -0x8000,  5, 0, -0x10000, -0x8000,  6, 0, 0x10000, -0x8000,  7, 0, 0x20000,
                                  0x8000,   8, 0, 0x20000,  -0x8000,  9, 0, 0x20000,  -0x4000 });
    bool32 grabbedPlayers[PLAYER_COUNT];
    Hitbox hitbox;
    uint16 aniFrames;
};

// Entity Class
struct EntityPhantomHand {
    RSDK_ENTITY
    StateMachine(state);
    int32 grabTimer;
    int32 timer;
    int32 timerDecreaseSpeed;
    int32 scaleAngle;
    Vector2 targetPos;
    Entity *parent;
    bool32 doScanlineCB;
    Animator handAnimator;
    Animator crystalAnimator;
    Animator shineAnimator;
};

// Object Struct
extern ObjectPhantomHand *PhantomHand;

// Standard Entity Events
void PhantomHand_Update(void);
void PhantomHand_LateUpdate(void);
void PhantomHand_StaticUpdate(void);
void PhantomHand_Draw(void);
void PhantomHand_Create(void *data);
void PhantomHand_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PhantomHand_EditorDraw(void);
void PhantomHand_EditorLoad(void);
#endif
void PhantomHand_Serialize(void);

// Extra Entity Functions
void PhantomHand_CheckPlayerGrab(int32 playerX, int32 playerY);

void PhantomHand_State_Summon(void);
void PhantomHand_State_Appear(void);
void PhantomHand_State_TryGrabPlayer(void);  // 1P
void PhantomHand_State_TryGrabPlayers(void); // 2P or more
void PhantomHand_State_GrabbedPlayer(void);
void PhantomHand_State_Clasp(void);
void PhantomHand_State_Crystalize(void);
void PhantomHand_State_Shine(void);
void PhantomHand_State_BreakApart(void);
void PhantomHand_State_Disappear(void);

#endif //! OBJ_PHANTOMHAND_H
