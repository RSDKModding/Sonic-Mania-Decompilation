#ifndef OBJ_DDWRECKER_H
#define OBJ_DDWRECKER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int camBoundL;
    int camBoundR;
    int bossBoundL;
    int bossBoundR;
    int bossBoundT;
    int xVelocityUnknown[3];
    TABLE(int angleOffsets1[6], { -64, -32, -16, 16, 32, 64 });
    TABLE(int angleOffsets2[6], { 32, 48, 64, 80, 96, 128 });
    uint16 spriteIndex;
    uint16 sfx_BossHit;
    uint16 sfx_Explosion;
    uint16 sfx_Drop;
    uint16 sfx_Impact;
    uint16 sfx_Assemble;
    uint16 sfx_Rotate;
    uint16 sfx_Sharp;
} ObjectDDWrecker;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(lateState);
    Vector2 startPos;
    int type;
    int timer;
    bool32 invincible;
    int health;
    int flag;
    int timer4;
    int timer2;
    int angle3;
    int arcOffset;
    uint16 slots[6];
    void *bodyA;
    void *bodyB;
    int field_A0;
    int angle2;
    int field_A8;
    int field_AC;
    int blendAmount;
    Animator animator;
    Hitbox hitbox;
} EntityDDWrecker;

// Object Struct
extern ObjectDDWrecker *DDWrecker;

// Standard Entity Events
void DDWrecker_Update(void);
void DDWrecker_LateUpdate(void);
void DDWrecker_StaticUpdate(void);
void DDWrecker_Draw(void);
void DDWrecker_Create(void* data);
void DDWrecker_StageLoad(void);
void DDWrecker_EditorDraw(void);
void DDWrecker_EditorLoad(void);
void DDWrecker_Serialize(void);

// Extra Entity Functions
void DDWrecker_State_Init(void);
void DDWrecker_State_InitChildren(void);
void DDWrecker_State_Assemble(void);
void DDWrecker_State_Unknown2(void);
void DDWrecker_State_Unknown3(void);
void DDWrecker_State_Unknown4(void);
void DDWrecker_State_Unknown5(void);
void DDWrecker_State_Unknown6(void);
void DDWrecker_State_Unknown7(void);
void DDWrecker_State_Unknown8(void);
void DDWrecker_State_Unknown9(void);
void DDWrecker_State_Unknown10(void);
void DDWrecker_State_Unknown11(void);
void DDWrecker_State_Unknown12(void);
void DDWrecker_LateState_Unknown1(void);
void DDWrecker_LateState_Unknown2(void);
void DDWrecker_LateState_Unknown3(void);
void DDWrecker_Hit(void);
void DDWrecker_Spin(void);
void DDWrecker_Spin2(void);
void DDWrecker_State_Debris(void);
void DDWrecker_State_Die(void);
void DDWrecker_State_SpawnSignpost(void);

#endif //!OBJ_DDWRECKER_H
