#ifndef OBJ_HOTARUHIWATT_H
#define OBJ_HOTARUHIWATT_H

#include "SonicMania.h"

typedef enum {
    HHW_0,
    HHW_1,
    HHW_2,
    HHW_3,
    HHW_4,
    HHW_5,
    HHW_6,
    HHW_MINILASER,
    HHW_ELECTRICORB,
}HHWTypes;

// Object Class
struct ObjectHotaruHiWatt {
    RSDK_OBJECT
    TABLE(int32 debrisInfo[33],
          { 8, 0, 0,        -0x20000, -0x28000, 1, 0,       0x20000,  -0x28000, 2, 0,        -0x28000, -0x20000, 3, 0,       0x28000, -0x20000,
            4, 0, -0x28000, -0x18000, 5,        0, 0x28000, -0x18000, 6,        0, -0x20000, -0x10000, 7,        0, 0x20000, -0x10000 });
    int32 boundsL;
    int32 boundsM;
    int32 boundsR;
    int32 boundsT;
    int32 boundsB;
    int32 value7;
    bool32 value8;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxHHWAppear;
    uint16 sfxFlash;
    uint16 sfxLaser;
    uint16 sfxAppear;
    uint16 sfxFly;
    uint16 sfxFlyUp;
    uint16 sfxCharge;
    uint16 aniFrames;
    uint16 hotaruFrames;
};

// Entity Class
struct EntityHotaruHiWatt {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(nextState);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 field_74;
    int32 timer2;
    int32 field_7C;
    Vector2 field_80;
    Vector2 field_88;
    Vector2 drawPos;
    Entity *parents[3];
    int32 field_A4;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Hitbox hitbox;
};

// Object Struct
extern ObjectHotaruHiWatt *HotaruHiWatt;

// Standard Entity Events
void HotaruHiWatt_Update(void);
void HotaruHiWatt_LateUpdate(void);
void HotaruHiWatt_StaticUpdate(void);
void HotaruHiWatt_Draw(void);
void HotaruHiWatt_Create(void* data);
void HotaruHiWatt_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HotaruHiWatt_EditorDraw(void);
void HotaruHiWatt_EditorLoad(void);
#endif
void HotaruHiWatt_Serialize(void);

// Extra Entity Functions
bool32 HotaruHiWatt_ZapCheckCB(void);
bool32 HotaruHiWatt_LaserCheckCB(void);
void HotaruHiWatt_LaserUpdateCB(int sfx);

void HotaruHiWatt_CheckPlayerCollisions2(void);
void HotaruHiWatt_CheckPlayerCollisions(void);
void HotaruHiWatt_CheckPlayerCollisions3(void);
void HotaruHiWatt_CheckPlayerCollisions4(void);

void HotaruHiWatt_StateDraw1_Unknown2(void);
void HotaruHiWatt_StateDraw1_Unknown1(void);
void HotaruHiWatt_StateDraw_Destroyed(void);
void HotaruHiWatt_StateDraw_Unknown1(void);
void HotaruHiWatt_StateDraw_Unknown2(void);
void HotaruHiWatt_StateDraw_Unknown3(void);
void HotaruHiWatt_StateDraw_Unknown4(void);

void HotaruHiWatt_SpawnChildHotaru(void);
void HotaruHiWatt_Unknown16(void);

void HotaruHiWatt_State_SetupBounds(void);
void HotaruHiWatt_State_WaitForPlayer(void);
void HotaruHiWatt_State_Unknown5(void);
void HotaruHiWatt_State_Unknown6(void);
void HotaruHiWatt_State1_Unknown2(void);

void HotaruHiWatt_State5_Unknown1(void);

void HotaruHiWatt_StateUnknown_Unknown1(void);
void HotaruHiWatt_StateUnknown_Unknown2(void);

void HotaruHiWatt_State1_Unknown1(void);
void HotaruHiWatt_State1_Unknown3(void);
void HotaruHiWatt_State1_Unknown4(void);
void HotaruHiWatt_State1_Unknown5(void);
void HotaruHiWatt_State1_Unknown6(void);

void HotaruHiWatt_State7_Unknown1(void);
void HotaruHiWatt_State7_Unknown2(void);

void HotaruHiWatt_State2_Unknown1(void);
void HotaruHiWatt_State2_Unknown2(void);
void HotaruHiWatt_State2_Unknown3(void);
void HotaruHiWatt_State2_Unknown4(void);
void HotaruHiWatt_State2_Unknown5(void);
void HotaruHiWatt_State2_Unknown7(void);

void HotaruHiWatt_State8_Unknown1(void);
void HotaruHiWatt_State8_Unknown2(void);

void HotaruHiWatt_State_Unknown1(void);
void HotaruHiWatt_State_Unknown2(void);
void HotaruHiWatt_State_Unknown3(void);
void HotaruHiWatt_State_Unknown4(void);
void HotaruHiWatt_State_Unknown7(void);
void HotaruHiWatt_State_Unknown8(void);
void HotaruHiWatt_State_Unknown9(void);
void HotaruHiWatt_State_Unknown10(void);
void HotaruHiWatt_State_Unknown11(void);
void HotaruHiWatt_State_Destroyed(void);
void HotaruHiWatt_State_Finished(void);

#endif //!OBJ_HOTARUHIWATT_H
