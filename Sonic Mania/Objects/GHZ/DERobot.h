#ifndef OBJ_DEROBOT_H
#define OBJ_DEROBOT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox2;
    Hitbox hitbox;
    uint16 field_16;
    uint16 field_18;
    uint16 field_1A;
    uint16 field_1C;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxTargeting;
    uint16 sfxImpact;
    uint16 sfxLedgeBreak;
    uint16 sfxBuzzsaw;
    uint16 sfxDrop;
    uint16 sfxButton2;
    uint16 sfxHullClose;
} ObjectDERobot;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 field_60;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    Entity *dword70;
    Entity *parts[13];
    int32 field_A8;
    int32 field_AC;
    int32 field_B0;
    int32 field_B4;
    Vector2 offset;
    int32 timer;
    int32 field_C4;
    int32 field_C8;
    int32 field_CC;
    int32 field_D0;
    int32 field_D4;
    int32 field_D8;
    int32 field_DC;
    Hitbox hitbox;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    int32 aniID;
    int32 frameID;
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
#if RETRO_INCLUDE_EDITOR
void DERobot_EditorDraw(void);
void DERobot_EditorLoad(void);
#endif
void DERobot_Serialize(void);

// Extra Entity Functions
void DERobot_Unknown1(void);
void DERobot_Unknown2(int32 a1);
void DERobot_Unknown3(int32 a1);
void DERobot_Unknown4(int32 a1);
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
