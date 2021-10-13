#ifndef OBJ_METALARM_H
#define OBJ_METALARM_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectMetalArm;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int startAngleA;
    int startAngleB;
    int endAngleA;
    int endAngleB;
    int durationA;
    int durationB;
    int holdDuration;
    Vector2 armAngle;
    uint8 activePlayers;
    Vector2 startPos;
    int timer;
    int timer2;
    Vector2 posUnknown;
    Vector2 offset;
    Hitbox hitbox;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
} EntityMetalArm;

// Object Struct
extern ObjectMetalArm *MetalArm;

// Standard Entity Events
void MetalArm_Update(void);
void MetalArm_LateUpdate(void);
void MetalArm_StaticUpdate(void);
void MetalArm_Draw(void);
void MetalArm_Create(void* data);
void MetalArm_StageLoad(void);
void MetalArm_EditorDraw(void);
void MetalArm_EditorLoad(void);
void MetalArm_Serialize(void);

// Extra Entity Functions
bool32 MetalArm_Unknown1(void);
Vector2 MetalArm_Unknown2(void);
void MetalArm_Unknown3(void);
void MetalArm_Unknown4(void);
void MetalArm_Unknown5(void);
void MetalArm_Unknown6(void);
void MetalArm_Unknown7(void);

#endif //!OBJ_METALARM_H
