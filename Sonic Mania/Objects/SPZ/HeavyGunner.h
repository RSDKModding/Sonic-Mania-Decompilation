#ifndef OBJ_HEAVYGUNNER_H
#define OBJ_HEAVYGUNNER_H

#include "SonicMania.h"

typedef enum {
    HEAVYGUNNER_HBH,
    HEAVYGUNNER_1,
    HEAVYGUNNER_HELI,
    HEAVYGUNNER_EGGROBO,
    HEAVYGUNNER_GUN,
    HEAVYGUNNER_MISSILE_F,
    HEAVYGUNNER_MISSILE,
    HEAVYGUNNER_ESCAPE_HBH,
}HeavyGunnerTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 boundsL;
    int32 boundsR;
    int32 boundsT;
    int32 boundsB;
    bool32 field_14;
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
    uint16 sfxFire;
    uint16 sfxJet;
    uint16 sfxHit;
    uint16 sfxDestroy;
    uint16 sfxExplosion2;
    uint16 sfxExplosion3;
    uint16 sfxCharge;
    uint16 sfxThrow;
    uint16 sfxFlyIn;
    uint16 sfxWooshIn;
    uint16 sfxWooshOut;
    uint16 sfxFlip;
    uint16 sfxRumble;
    int32 field_44;
    int32 field_48;
    int32 field_4C;
    int32 field_50;
} ObjectHeavyGunner;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Entity *parent;
    int32 timer;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 type;
    int32 field_7C;
    int32 field_80;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
    Animator animator7;
    Animator animator8;
    Animator animator9;
    Animator animator10;
    Animator animator11;
} EntityHeavyGunner;

// Object Struct
extern ObjectHeavyGunner *HeavyGunner;

// Standard Entity Events
void HeavyGunner_Update(void);
void HeavyGunner_LateUpdate(void);
void HeavyGunner_StaticUpdate(void);
void HeavyGunner_Draw(void);
void HeavyGunner_Create(void* data);
void HeavyGunner_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HeavyGunner_EditorDraw(void);
void HeavyGunner_EditorLoad(void);
#endif
void HeavyGunner_Serialize(void);

// Extra Entity Functions
bool32 HeavyGunner_HelicopterCheckCB(void);
bool32 HeavyGunner_SurpriseCheckCB(void);
bool32 HeavyGunner_BurnCheckCB(void);

void HeavyGunner_HandleBGWrap(int multiplier);
void HeavyGunner_Unknown5(void);

void HeavyGunner_StateDraw_Unknown2(void);
void HeavyGunner_StateDraw3_Unknown(void);
void HeavyGunner_StateDraw6_Unknown(void);
void HeavyGunner_StateDraw4_Unknown1(void);
void HeavyGunner_StateDraw4_Unknown2(void);

void HeavyGunner_State_SetupArena(void);
void HeavyGunner_State_Unknown3(void);
void HeavyGunner_State_Unknown2(void);

void HeavyGunner_InputState_LockedP1(void);
void HeavyGunner_InputState_LockedP2(void);
void HeavyGunner_InputState_LockedP2_AI(void);

void HeavyGunner_State3_Unknown1(void);
void HeavyGunner_State3_Unknown2(void);
void HeavyGunner_State3_Unknown3(void);
void HeavyGunner_State3_Unknown4(void);

void HeavyGunner_State4_Unknown1(void);
void HeavyGunner_State4_Unknown2(void);

void HeavyGunner_State6_Unknown1(void);
void HeavyGunner_State6_Unknown2(void);
void HeavyGunner_State6_Unknown3(void);
void HeavyGunner_State6_Unknown4(void);
void HeavyGunner_State6_Unknown5(void);
void HeavyGunner_State6_Unknown6(void);
void HeavyGunner_State6_Unknown7(void);
void HeavyGunner_State6_Unknown8(void);

void HeavyGunner_State2_Unknown1(void);
void HeavyGunner_State2_Unknown2(void);
void HeavyGunner_State2_Unknown3(void);
void HeavyGunner_State2_Unknown4(void);
void HeavyGunner_State2_Unknown5(void);
void HeavyGunner_State2_Unknown6(void);
void HeavyGunner_State2_Unknown7(void);
void HeavyGunner_State2_Unknown8(void);
void HeavyGunner_State2_Unknown9(void);

void HeavyGunner_State7_Unknown1(void);
void HeavyGunner_State7_Unknown2(void);

void HeavyGunner_State2_Unknown10(void);

#endif //!OBJ_HEAVYGUNNER_H
