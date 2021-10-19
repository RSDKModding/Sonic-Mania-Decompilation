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
    int32 field_60;
    Vector2 ballPos[12];
    Vector2 clawPos;
    Entity *grabbedEntities[8];
    int32 count;
    int32 field_F0;
    Animator animator1;
    Animator animator2;
    Animator data3;
    Animator data4;
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
void AIZKingClaw_EditorDraw(void);
void AIZKingClaw_EditorLoad(void);
void AIZKingClaw_Serialize(void);

// Extra Entity Functions
void AIZKingClaw_Unknown1(void);
void AIZKingClaw_Unknown2(void);

#endif //!OBJ_AIZKINGCLAW_H
