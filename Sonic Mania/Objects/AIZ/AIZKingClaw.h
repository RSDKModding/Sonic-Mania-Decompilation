#ifndef OBJ_AIZKINGCLAW_H
#define OBJ_AIZKINGCLAW_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxClack;
    uint16 sfxWalkerLegs;
} ObjectAIZKingClaw;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 startY;
    Vector2 ballPos[12];
    Vector2 clawPos;
    Entity *grabbedEntities[8];
    int32 grabCount;
    int32 unused1;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
} EntityAIZKingClaw;

// Object Struct
extern ObjectAIZKingClaw *AIZKingClaw;

// Standard Entity Events
void AIZKingClaw_Update(void);
void AIZKingClaw_LateUpdate(void);
void AIZKingClaw_StaticUpdate(void);
void AIZKingClaw_Draw(void);
void AIZKingClaw_Create(void* data);
void AIZKingClaw_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void AIZKingClaw_EditorDraw(void);
void AIZKingClaw_EditorLoad(void);
#endif
void AIZKingClaw_Serialize(void);

// Extra Entity Functions
void AIZKingClaw_HandleClawPositions(void);
void AIZKingClaw_State_Grab(void);

#endif //!OBJ_AIZKINGCLAW_H
