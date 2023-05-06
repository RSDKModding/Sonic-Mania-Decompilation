#ifndef OBJ_METALARM_H
#define OBJ_METALARM_H

#include "Game.h"

// Object Class
struct ObjectMetalArm {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityMetalArm {
    RSDK_ENTITY
    StateMachine(state);
    int32 startAngleA;
    int32 startAngleB;
    int32 endAngleA;
    int32 endAngleB;
    int32 durationA;
    int32 durationB;
    int32 holdDuration;
    Vector2 armAngle;
    uint8 stoodPlayers;
    Vector2 startPos;
    int32 moveTimer;
    int32 holdTimer;
    Vector2 armPosition;
    Vector2 moveOffset;
    Hitbox hitbox;
    Animator baseAnimator;
    Animator armAAnimator;
    Animator armBAnimator;
    Animator platformAnimator;
};

// Object Struct
extern ObjectMetalArm *MetalArm;

// Standard Entity Events
void MetalArm_Update(void);
void MetalArm_LateUpdate(void);
void MetalArm_StaticUpdate(void);
void MetalArm_Draw(void);
void MetalArm_Create(void *data);
void MetalArm_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MetalArm_EditorDraw(void);
void MetalArm_EditorLoad(void);
#endif
void MetalArm_Serialize(void);

// Extra Entity Functions
bool32 MetalArm_SfxCheck_MetalArm(void);
Vector2 MetalArm_GetArmPosition(void);
void MetalArm_CheckPlayerCollisions(void);
void MetalArm_State_Idle(void);
void MetalArm_State_MoveToHold(void);
void MetalArm_State_Holding(void);
void MetalArm_State_MoveToStart(void);

#endif //! OBJ_METALARM_H
