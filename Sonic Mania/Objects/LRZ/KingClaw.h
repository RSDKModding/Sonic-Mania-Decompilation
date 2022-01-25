#ifndef OBJ_KINGCLAW_H
#define OBJ_KINGCLAW_H

#include "SonicMania.h"

#define KingClaw_JointCount (12)

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
    Vector2 jointPos[KingClaw_JointCount];
    Vector2 drawPos;
    EntityHPZEmerald *masterEmerald;
    bool32 flag;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
};

// Object Struct
extern ObjectKingClaw *KingClaw;

// Standard Entity Events
void KingClaw_Update(void);
void KingClaw_LateUpdate(void);
void KingClaw_StaticUpdate(void);
void KingClaw_Draw(void);
void KingClaw_Create(void* data);
void KingClaw_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void KingClaw_EditorDraw(void);
void KingClaw_EditorLoad(void);
#endif
void KingClaw_Serialize(void);

// Extra Entity Functions
void KingClaw_HandleJointPositions(void);

void KingClaw_Unknown2(void);
void KingClaw_Unknown3(void);
void KingClaw_Unknown4(void);
void KingClaw_Unknown5(void);
void KingClaw_Unknown6(void);
void KingClaw_Unknown7(void);

#endif //!OBJ_KINGCLAW_H
