#ifndef OBJ_METERDROID_H
#define OBJ_METERDROID_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int debrisSpeeds[21], { 5, 0, 0, -0x20000, -0x28000, 1, 0, 0x20000, -0x20000, 2, 0, -0x28000, -0x10000, 3, 0, 0x28000, -0x10000, 4, 0, 0x10000, 0x10000 });
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    int boundsL;
    int boundsR;
    int startX;
    int boundsT;
    int boundsB;
    ushort sfxHit;
    ushort sfxExplosion;
    ushort sfxDestroy;
    ushort sfxToss;
    ushort sfxGrab;
    ushort sfxValve;
    ushort sfxWrench;
    ushort aniFrames;
} ObjectMeterDroid;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int timer;
    int invincibilityTimer;
    int health;
    int field_6C;
    Vector2 field_70;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    Vector2 field_88;
    Vector2 field_90;
    Vector2 field_98;
    int field_A0;
    Entity *valvePtr;
    Animator animator1;
    Animator animator2;
    Animator animator4;
    Animator animator3;
} EntityMeterDroid;

// Object Struct
extern ObjectMeterDroid *MeterDroid;

// Standard Entity Events
void MeterDroid_Update(void);
void MeterDroid_LateUpdate(void);
void MeterDroid_StaticUpdate(void);
void MeterDroid_Draw(void);
void MeterDroid_Create(void* data);
void MeterDroid_StageLoad(void);
void MeterDroid_EditorDraw(void);
void MeterDroid_EditorLoad(void);
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
