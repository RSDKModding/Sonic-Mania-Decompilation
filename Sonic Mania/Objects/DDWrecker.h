#ifndef OBJ_DDWRECKER_H
#define OBJ_DDWRECKER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int xVelocityUnknown[3];
    int array1[6];
    int array2[6];
    ushort spriteIndex;
    ushort sfx_BossHit;
    ushort sfx_Explosion;
    ushort sfx_Drop;
    ushort sfx_Impact;
    ushort sfx_Assemble;
    ushort sfx_Rotate;
    ushort sfx_Sharp;
} ObjectDDWrecker;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void(*state)();
    void(*lateState)();
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
    ushort slots[6];
    void *bodyA;
    void *bodyB;
    int field_A0;
    int angle2;
    int field_A8;
    int field_AC;
    int blendAmount;
    AnimationData animData;
    Hitbox hitbox;
} EntityDDWrecker;

// Object Struct
extern ObjectDDWrecker *DDWrecker;

// Standard Entity Events
void DDWrecker_Update();
void DDWrecker_LateUpdate();
void DDWrecker_StaticUpdate();
void DDWrecker_Draw();
void DDWrecker_Create(void* data);
void DDWrecker_StageLoad();
void DDWrecker_EditorDraw();
void DDWrecker_EditorLoad();
void DDWrecker_Serialize();

// Extra Entity Functions
void DDWrecker_State_Init();
void DDWrecker_State_InitChildren();
void DDWrecker_State_Assemble();
void DDWrecker_State_Unknown2();
void DDWrecker_State_Unknown3();
void DDWrecker_State_Unknown4();
void DDWrecker_State_Unknown5();
void DDWrecker_State_Unknown6();
void DDWrecker_State_Unknown7();
void DDWrecker_State_Unknown8();
void DDWrecker_State_Unknown9();
void DDWrecker_State_Unknown10();
void DDWrecker_State_Unknown11();
void DDWrecker_State_Unknown12();
void DDWrecker_LateState_Unknown1();
void DDWrecker_LateState_Unknown2();
void DDWrecker_LateState_Unknown3();
void DDWrecker_Hit();
void DDWrecker_Spin();
void DDWrecker_Spin2();
void DDWrecker_State_Debris();
void DDWrecker_State_Die();
void DDWrecker_State_SpawnSignpost();

#endif //!OBJ_DDWRECKER_H
