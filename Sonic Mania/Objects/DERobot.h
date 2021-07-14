#ifndef OBJ_DEROBOT_H
#define OBJ_DEROBOT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    Hitbox hitbox2;
    Hitbox hitbox;
    ushort field_16;
    ushort field_18;
    ushort field_1A;
    ushort field_1C;
    ushort sfxHit;
    ushort sfxExplosion;
    ushort sfxTargeting;
    ushort sfxImpact;
    ushort sfxLedgeBreak;
    ushort sfxBuzzsaw;
    ushort sfxDrop;
    ushort sfxButton2;
    ushort sfxHullClose;
} ObjectDERobot;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    Entity *dword70;
    Entity *parts[13];
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    Vector2 offset;
    int timer;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    Hitbox hitbox;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    int aniID;
    int frameID;
} EntityDERobot;

// Object Struct
extern ObjectDERobot *DERobot;

// Standard Entity Events
void DERobot_Update(void);
void DERobot_LateUpdate(void);
void DERobot_StaticUpdate(void);
void DERobot_Draw(void);
void DERobot_Create(void* data);
void DERobot_StageLoad(void);
void DERobot_EditorDraw(void);
void DERobot_EditorLoad(void);
void DERobot_Serialize(void);

// Extra Entity Functions
void DERobot_Unknown1(void);
void DERobot_Unknown2(int a1);
void DERobot_Unknown3(int a1);
void DERobot_Unknown4(int a1);
void DERobot_Unknown5(void);
void DERobot_Unknown6(void);
void DERobot_Unknown7(void);
void DERobot_Unknown8(void);
void DERobot_Unknown9(void);
void DERobot_Unknown10(void);
bool32 DERobot_Unknown11(void);
void DERobot_Unknown12(void);

void DERobot_StateDraw_Unknown1(void);
void DERobot_StateDraw_Unknown2(void);
void DERobot_StateDraw_Unknown3(void);
void DERobot_StateDraw_Unknown4(void);
void DERobot_StateDraw_Unknown5(void);

void DERobot_Unknown18(void);
void DERobot_Unknown19(void);
void DERobot_Unknown20(void);
void DERobot_Unknown21(void);
void DERobot_Unknown22(void);
void DERobot_Unknown23(void);
void DERobot_Unknown24(void);
void DERobot_Unknown25(void);
void DERobot_Unknown26(void);
void DERobot_Unknown27(void);
void DERobot_Unknown28(void);
void DERobot_Unknown29(void);
void DERobot_Unknown30(void);
void DERobot_Unknown31(void);
void DERobot_Unknown32(void);
void DERobot_Unknown33(void);
void DERobot_Unknown34(void);
void DERobot_Unknown35(void);
void DERobot_Unknown36(void);
void DERobot_Unknown37(void);
void DERobot_Unknown38(void);
void DERobot_Unknown39(void);
void DERobot_Unknown40(void);
void DERobot_Unknown41(void);
void DERobot_Unknown42(void);
void DERobot_Unknown43(void);

#endif //!OBJ_DEROBOT_H
