#ifndef OBJ_MONKEYDUDE_H
#define OBJ_MONKEYDUDE_H

#include "Game.h"

#define MonkeyDude_ArmJointCount (4)

// Object Class
struct ObjectMonkeyDude {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxCoconut;
    Animator armAnimator;
    Animator tailAnimator;
    Animator coconutAnimator;
    uint16 aniFrames;
#if MANIA_USE_PLUS
    uint16 sfxDrop;
#endif
};

// Entity Class
struct EntityMonkeyDude {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 activeArmNodes;
    int32 currentAngle;
    StateMachine(armStates[MonkeyDude_ArmJointCount]);
    int32 armAngles[MonkeyDude_ArmJointCount];
    int32 armMoveDir[MonkeyDude_ArmJointCount];
    int32 armTimers[MonkeyDude_ArmJointCount];
    uint8 armNodeID;
    Vector2 startPos;
    uint8 startDir;
    int32 armY;
    int32 moveCount;
    int32 nummoves;
    int32 throwCount;
    int32 angleVel;
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
void MonkeyDude_Create(void *data);
void MonkeyDude_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MonkeyDude_EditorDraw(void);
void MonkeyDude_EditorLoad(void);
#endif
void MonkeyDude_Serialize(void);

// Extra Entity Functions
void MonkeyDude_DebugDraw(void);
void MonkeyDude_DebugSpawn(void);

void MonkeyDude_State_Init(void);
void MonkeyDude_HandleBodyPart(void);
#if MANIA_USE_PLUS
void MonkeyDude_HandleStates(void);
#endif
void MonkeyDude_State_Laugh(void);
void MonkeyDude_State_MoveArm(void);
void MonkeyDude_State_MoveBody(void);
void MonkeyDude_StateBody_ArmRaise(void);
void MonkeyDude_StateBody_Throw(void);
#if MANIA_USE_PLUS
void MonkeyDude_State_Coconut(void);
#endif

#endif //! OBJ_MONKEYDUDE_H
