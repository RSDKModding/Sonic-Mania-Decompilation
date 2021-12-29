#ifndef OBJ_METERDROID_H
#define OBJ_METERDROID_H

#include "SonicMania.h"

// Object Class
struct ObjectMeterDroid {
    RSDK_OBJECT
    TABLE(int32 debrisSpeeds[21], { 5, 0, 0, -0x20000, -0x28000, 1, 0, 0x20000, -0x20000, 2, 0, -0x28000, -0x10000, 3, 0, 0x28000, -0x10000, 4, 0, 0x10000, 0x10000 });
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    int32 boundsL;
    int32 boundsR;
    int32 startX;
    int32 boundsT;
    int32 boundsB;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxDestroy;
    uint16 sfxToss;
    uint16 sfxGrab;
    uint16 sfxValve;
    uint16 sfxWrench;
    uint16 aniFrames;
};

// Entity Class
struct EntityMeterDroid {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 field_6C;
    Vector2 field_70;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    Vector2 field_88;
    Vector2 field_90;
    Vector2 field_98;
    int32 field_A0;
    Entity *valvePtr;
    Animator animator1;
    Animator animator2;
    Animator animator4;
    Animator animator3;
};

// Object Struct
extern ObjectMeterDroid *MeterDroid;

// Standard Entity Events
void MeterDroid_Update(void);
void MeterDroid_LateUpdate(void);
void MeterDroid_StaticUpdate(void);
void MeterDroid_Draw(void);
void MeterDroid_Create(void* data);
void MeterDroid_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MeterDroid_EditorDraw(void);
void MeterDroid_EditorLoad(void);
#endif
void MeterDroid_Serialize(void);

// Extra Entity Functions
void MeterDroid_CheckPlayerCollisions(void);
void MeterDroid_CheckPlayerCollisions2(void);
void MeterDroid_CheckPlayerCollisions3(void);
void MeterDroid_Hit(void);
void MeterDroid_Unknown5(void);
void MeterDroid_PopPlatforms(void);

void MeterDroid_StateDraw_Unknown1(void);
void MeterDroid_StateDraw_Unknown3(void);
void MeterDroid_StateDraw_Unknown2(void);

void MeterDroid_State_Setup(void);
void MeterDroid_State_Unknown1(void);
void MeterDroid_State_Unknown2(void);
void MeterDroid_State_Unknown3(void);
void MeterDroid_State_Unknown4(void);
void MeterDroid_State_Unknown5(void);
void MeterDroid_State_Unknown6(void);
void MeterDroid_State_Unknown7(void);
void MeterDroid_State_Unknown8(void);
void MeterDroid_State_Unknown9(void);
void MeterDroid_State_Unknown10(void);
void MeterDroid_State_Unknown11(void);
void MeterDroid_State_Unknown12(void);
void MeterDroid_State_Unknown13(void);
void MeterDroid_State_Die(void);
void MeterDroid_State_FinishAct(void);

#endif //!OBJ_METERDROID_H
