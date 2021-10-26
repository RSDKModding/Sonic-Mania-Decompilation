#ifndef OBJ_ERZGUNNER_H
#define OBJ_ERZGUNNER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 value1[32], { -22, -24, -10, -24, -22, -31, -10, -31, -22, -38, -10, -38, -22, -45, -10, -45, 10, -24, 22, -24, 10, -31, 22, -31, 10, -38, 22, -38, 10, -45, 22, -45 });
    int32 field_84;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    uint16 aniFrames;
} ObjectERZGunner;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 posUnknown1;
    int32 type;
    Entity *parent;
    int32 timer2;
    int32 field_74;
    int32 timer3;
    int32 timer;
    int32 field_80;
    int32 field_84;
    Vector2 posUnknown2;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
} EntityERZGunner;

// Object Struct
extern ObjectERZGunner *ERZGunner;

// Standard Entity Events
void ERZGunner_Update(void);
void ERZGunner_LateUpdate(void);
void ERZGunner_StaticUpdate(void);
void ERZGunner_Draw(void);
void ERZGunner_Create(void* data);
void ERZGunner_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ERZGunner_EditorDraw(void);
void ERZGunner_EditorLoad(void);
#endif
void ERZGunner_Serialize(void);

// Extra Entity Functions
void ERZGunner_Explode(void);
void ERZGunner_Explode2(void);
void ERZGunner_SpawnDust(void);
void ERZGunner_HandleRotations(int angle);
void ERZGunner_CheckPlayerMissileCollisions(void);
void ERZGunner_CheckPlayerExplosionCollisions(void);

void ERZGunner_StateDraw_Unknown0(void);
void ERZGunner_StateDraw_Unknown1(void);
void ERZGunner_StateDraw_Unknown2(void);

void ERZGunner_State_Unknown1(void);
void ERZGunner_State_Unknown2(void);

void ERZGunner_State1_Unknown1(void);
void ERZGunner_State1_Unknown2(void);
void ERZGunner_State1_Unknown3(void);
void ERZGunner_State1_Unknown4(void);
void ERZGunner_State1_Unknown5(void);
void ERZGunner_State1_Unknown6(void);
void ERZGunner_State1_Unknown7(void);

void ERZGunner_State2_Unknown(void);
void ERZGunner_State3_Unknown(void);

#endif //!OBJ_ERZGUNNER_H
