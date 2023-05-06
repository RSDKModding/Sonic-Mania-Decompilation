#ifndef OBJ_AIZKINGCLAW_H
#define OBJ_AIZKINGCLAW_H

#include "Game.h"

#define AIZKingClaw_ChainCount (12)

// Object Class
struct ObjectAIZKingClaw {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxClack;
    uint16 sfxWalkerLegs;
};

// Entity Class
struct EntityAIZKingClaw {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 startY;
    Vector2 chainPos[AIZKingClaw_ChainCount];
    Vector2 clawPos;
    Entity *grabbedEntities[8];
    int32 grabCount;
    int32 unused1;
    Animator chainAnimator;
    Animator hingeAnimator;
    Animator clawBackAnimator;
    Animator clawFrontAnimator;
};

// Object Struct
extern ObjectAIZKingClaw *AIZKingClaw;

// Standard Entity Events
void AIZKingClaw_Update(void);
void AIZKingClaw_LateUpdate(void);
void AIZKingClaw_StaticUpdate(void);
void AIZKingClaw_Draw(void);
void AIZKingClaw_Create(void *data);
void AIZKingClaw_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void AIZKingClaw_EditorDraw(void);
void AIZKingClaw_EditorLoad(void);
#endif
void AIZKingClaw_Serialize(void);

// Extra Entity Functions
void AIZKingClaw_HandleClawPositions(void);
void AIZKingClaw_State_Grab(void);

#endif //! OBJ_AIZKINGCLAW_H
