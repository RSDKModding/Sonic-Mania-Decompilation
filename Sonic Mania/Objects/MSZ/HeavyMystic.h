#ifndef OBJ_HEAVYMYSTIC_H
#define OBJ_HEAVYMYSTIC_H

#include "SonicMania.h"

typedef enum {
    MYSTIC_MISCHIEF,
    MYSTIC_BOSS,
    MYSTIC_BOX,
    MYSTIC_CORK,
    MYSTIC_BOMB,
    MYSTIC_DEBRIS,
}HeavyMysticTypes;

// Object Class
struct ObjectHeavyMystic {
    RSDK_OBJECT
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    int32 curtainLinePos;
    uint16 sfxAssemble;
    uint16 sfxMagicBox;
    uint16 sfxClack;
    uint16 sfxPowerDown;
    uint16 sfxImpact;
    uint16 sfxPon;
    uint16 sfxDrop;
    uint16 sfxImpact2;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxPoof;
    uint16 sfxTwinkle;
    uint16 sfxHat;
    uint16 sfxHatNode;
    uint16 sfxBleeps;
    uint16 sfxTransform;
    uint16 sfxAppear1;
    uint16 aniFrames;
    uint16 rouguesFrames;
};

// Entity Class
struct EntityHeavyMystic {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 rougeHealth;
    int32 timer2;
    int32 field_78;
    int32 field_7C;
    int32 rougeID;
    Vector2 targetPos;
    Animator animator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectHeavyMystic *HeavyMystic;

// Standard Entity Events
void HeavyMystic_Update(void);
void HeavyMystic_LateUpdate(void);
void HeavyMystic_StaticUpdate(void);
void HeavyMystic_Draw(void);
void HeavyMystic_Create(void* data);
void HeavyMystic_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HeavyMystic_EditorDraw(void);
void HeavyMystic_EditorLoad(void);
#endif
void HeavyMystic_Serialize(void);

// Extra Entity Functions
void HeavyMystic_Unknown1(int32 x, int32 y);
void HeavyMystic_Unknown2(void);

void HeavyMystic_CheckPlayerCollisions(void);
void HeavyMystic_Hit(void);
void HeavyMystic_CheckPlayerCollisions2(void);
void HeavyMystic_CheckPlayerCollisions3(void);
void HeavyMystic_CheckPlayerCollisions4(void);
void HeavyMystic_Explode(void);

void HeavyMystic_ScanlineCB(ScanlineInfo *scanlines);

void HeavyMystic_State0_Unknown1(void);
void HeavyMystic_State1_Unknown1(void);
void HeavyMystic_State1_Unknown2(void);
void HeavyMystic_State0_Unknown2(void);
void HeavyMystic_State0_Unknown3(void);
void HeavyMystic_State1_Unknown3(void);
void HeavyMystic_State0_Unknown7(void);
void HeavyMystic_State0_Unknown4(void);
void HeavyMystic_State0_Unknown9(void);
void HeavyMystic_State0_Unknown6(void);
void HeavyMystic_State0_Unknown5(void);
void HeavyMystic_State0_Unknown8(void);
void HeavyMystic_State0_Unknown10(void);

void HeavyMystic_State_Destroyed(void);
void HeavyMystic_State_Finish(void);
void HeavyMystic_State_CloseCurtains(void);
void HeavyMystic_State1_Unknown4(void);
void HeavyMystic_State1_Unknown5(void);
void HeavyMystic_State1_Unknown6(void);
void HeavyMystic_State1_Unknown7(void);
void HeavyMystic_State1_Unknown8(void);
void HeavyMystic_State1_Unknown9(void);
void HeavyMystic_State1_Unknown10(void);
void HeavyMystic_State1_ShowRouge(void);
void HeavyMystic_State1_MysticReveal(void);
void HeavyMystic_State1_Unknown22(void);
void HeavyMystic_State1_Unknown23(void);
void HeavyMystic_State1_Unknown24(void);
void HeavyMystic_State1_Unknown13(void);
void HeavyMystic_State1_Unknown14(void);
void HeavyMystic_State1_Unknown12(void);
void HeavyMystic_State_RougeHit(void);
void HeavyMystic_State1_Unknown20(void);
void HeavyMystic_State1_Unknown21(void);
void HeavyMystic_State1_Unknown19(void);
void HeavyMystic_State1_Unknown16(void);
void HeavyMystic_State1_Unknown17(void);
void HeavyMystic_State1_Unknown18(void);
void HeavyMystic_State1_Unknown15(void);

void HeavyMystic_State3_Unknown1(void);
void HeavyMystic_State3_Unknown2(void);

void HeavyMystic_State4_Unknown1(void);

void HeavyMystic_State5_Unknown1(void);

void HeavyMystic_State2_Unknown1(void);
void HeavyMystic_State2_Unknown2(void);
void HeavyMystic_State2_Unknown4(void);
void HeavyMystic_State2_Unknown3(void);
void HeavyMystic_State2_Unknown5(void);
void HeavyMystic_State2_Unknown6(void);
void HeavyMystic_State2_Unknown7(void);
void HeavyMystic_State2_Unknown8(void);
void HeavyMystic_State2_Unknown9(void);

void HeavyMystic_StateDraw2_Unknown1(void);
void HeavyMystic_StateDraw2_Unknown2(void);
void HeavyMystic_StateDraw2_Unknown3(void);

#endif //!OBJ_HEAVYMYSTIC_H
