#ifndef OBJ_KINGCLAW_H
#define OBJ_KINGCLAW_H

#include "Game.h"

#define KINGCLAW_CHAIN_COUNT (12)

// Object Class
struct ObjectKingClaw {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxClack;
    uint16 sfxWalkerLegs;
};

// Entity Class
struct EntityKingClaw {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 targetY;
    Vector2 chainPos[KINGCLAW_CHAIN_COUNT];
    Vector2 drawPos;
    EntityHPZEmerald *masterEmerald;
    bool32 forceHighdrawGroup;
    Animator chainAnimator;
    Animator hingeAnimator;
    Animator clawBackAnimator;
    Animator clawFrontAnimator;
};

// Object Struct
extern ObjectKingClaw *KingClaw;

// Standard Entity Events
void KingClaw_Update(void);
void KingClaw_LateUpdate(void);
void KingClaw_StaticUpdate(void);
void KingClaw_Draw(void);
void KingClaw_Create(void *data);
void KingClaw_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void KingClaw_EditorDraw(void);
void KingClaw_EditorLoad(void);
#endif
void KingClaw_Serialize(void);

// Extra Entity Functions
void KingClaw_HandleJointPositions(void);

void KingClaw_State_EnterClaw(void);
void KingClaw_State_Grab(void);
void KingClaw_State_LiftMasterEmerald(void);
void KingClaw_State_Swinging(void);
void KingClaw_State_LowerClaw(void);
void KingClaw_State_RaiseClaw(void);

#endif //! OBJ_KINGCLAW_H
