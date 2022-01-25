#ifndef OBJ_MONKEYDUDE_H
#define OBJ_MONKEYDUDE_H

#include "SonicMania.h"

#define MonkeyDude_MaxBodyParts (4)

// Object Class
struct ObjectMonkeyDude {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox coconutHitbox;
    Animator armAnimator;
    Animator tailAnimator;
    Animator coconutAnimator;
    uint16 aniFrames;
#if RETRO_USE_PLUS
    uint16 sfxDrop;
#endif
};

// Entity Class
struct EntityMonkeyDude {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 activeParts;
    int32 currentAngle;
    StateMachine(bodyStates[MonkeyDude_MaxBodyParts]);
    int32 bodyAngles[MonkeyDude_MaxBodyParts];
    int32 bodyFlags[MonkeyDude_MaxBodyParts];
    int32 bodyTimers[MonkeyDude_MaxBodyParts];
    uint8 bodyPartID;
    Vector2 startPos;
    uint8 startDir;
    int32 armY;
    int32 moveCount;
    int32 nummoves;
    int32 throwCount;
    int32 angleSpeed;
    Animator bodyAnimator;
    Animator handAnimator;
    int32 coconutFrame;
};

// Object Struct
extern ObjectMonkeyDude *MonkeyDude;

// Standard Entity Events
void MonkeyDude_Update(void);
void MonkeyDude_LateUpdate(void);
void MonkeyDude_StaticUpdate(void);
void MonkeyDude_Draw(void);
void MonkeyDude_Create(void* data);
void MonkeyDude_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MonkeyDude_EditorDraw(void);
void MonkeyDude_EditorLoad(void);
#endif
void MonkeyDude_Serialize(void);

// Extra Entity Functions
void MonkeyDude_DebugDraw(void);
void MonkeyDude_DebugSpawn(void);

void MonkeyDude_State_Setup(void);
void MonkeyDude_HandleBodyPart(void);
void MonkeyDude_HandleStates(void);
void MonkeyDude_State_Laugh(void);
void MonkeyDude_State_MoveArm(void);
void MonkeyDude_State_Body(void);
void MonkeyDude_StateBody_ArmRaise(void);
void MonkeyDude_StateBody_Throw(void);
#if RETRO_USE_PLUS
void MonkeyDude_State_Coconut(void);
#endif

#endif //!OBJ_MONKEYDUDE_H
