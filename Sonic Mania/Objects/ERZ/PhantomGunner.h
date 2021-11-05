#ifndef OBJ_PHANTOMGUNNER_H
#define OBJ_PHANTOMGUNNER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 value1[32], { -22, -24, -10, -24, -22, -31, -10, -31, -22, -38, -10, -38, -22, -45, -10, -45,
                              10,  -24, 22,  -24, 10,  -31, 22,  -31, 10,  -38, 22,  -38, 10,  -45, 22,  -45 });
    int32 value2;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    uint16 aniFrames;
    uint16 sfxCannonFire;
} ObjectPhantomGunner;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 posUnknown1;
    Vector2 startPos;
    int32 type;
    Entity *parent;
    int32 timer2;
    int32 invincibilityTimer;
    int32 field_80;
    int32 timer3;
    int32 timer;
    int32 field_8C;
    int32 field_90;
    Vector2 posUnknown2;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
} EntityPhantomGunner;

// Object Struct
extern ObjectPhantomGunner *PhantomGunner;

// Standard Entity Events
void PhantomGunner_Update(void);
void PhantomGunner_LateUpdate(void);
void PhantomGunner_StaticUpdate(void);
void PhantomGunner_Draw(void);
void PhantomGunner_Create(void* data);
void PhantomGunner_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PhantomGunner_EditorDraw(void);
void PhantomGunner_EditorLoad(void);
#endif
void PhantomGunner_Serialize(void);

// Extra Entity Functions
void PhantomGunner_Explode(void);
void PhantomGunner_Explode2(void);
void PhantomGunner_SpawnDust(void);
void PhantomGunner_HandleRotations(int angle);
void PhantomGunner_CheckPlayerMissileCollisions(void);
void PhantomGunner_CheckPlayerExplosionCollisions(void);

void PhantomGunner_StateDraw_Unknown0(void);
void PhantomGunner_StateDraw_Unknown1(void);
void PhantomGunner_StateDraw_Unknown2(void);

void PhantomGunner_State_ResetState(void);
void PhantomGunner_State_Unknown1(void);
void PhantomGunner_State_Unknown2(void);

void PhantomGunner_State1_Unknown1(void);
void PhantomGunner_State1_Unknown2(void);
void PhantomGunner_State1_Unknown3(void);
void PhantomGunner_State1_Unknown4(void);
void PhantomGunner_State1_Unknown5(void);
void PhantomGunner_State1_Unknown6(void);
void PhantomGunner_State1_Unknown7(void);

void PhantomGunner_State2_Unknown(void);
void PhantomGunner_State3_Unknown(void);


#endif //!OBJ_PHANTOMGUNNER_H
