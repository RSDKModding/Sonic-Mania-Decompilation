#ifndef OBJ_SHIVERSAW_H
#define OBJ_SHIVERSAW_H

#include "SonicMania.h"

#define Shiversaw_SawCount (2)

// Object Class
typedef struct {
    RSDK_OBJECT
    uint8 health;
    uint8 invincibilityTimer;
    int32 timer;
    uint16 aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitboxR;
    Hitbox hitboxB;
    Hitbox hitboxT;
    Hitbox hitboxL;
#if RETRO_USE_PLUS
    Hitbox hitbox7;
    Hitbox hitbox8;
#endif
    uint16 sfxHit;
    uint16 sfxExplosion2;
    uint16 sfxTargeting;
    uint16 sfxRocketJet;
    uint16 sfxExplosion3;
} ObjectShiversaw;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateSaw[Shiversaw_SawCount]);
    uint8 sawID;
    int32 timer;
    int32 sawTimers[Shiversaw_SawCount];
    int32 field_74[Shiversaw_SawCount];
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    Vector2 field_88;
    int32 field_90[Shiversaw_SawCount];
    int32 sawAngles[Shiversaw_SawCount];
    int32 field_A0[Shiversaw_SawCount];
    Vector2 sawPos[Shiversaw_SawCount];
    int32 field_B8;
    int32 circleRadius;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator sawAnimator[Shiversaw_SawCount];
} EntityShiversaw;

// Object Struct
extern ObjectShiversaw *Shiversaw;

// Standard Entity Events
void Shiversaw_Update(void);
void Shiversaw_LateUpdate(void);
void Shiversaw_StaticUpdate(void);
void Shiversaw_Draw(void);
void Shiversaw_Create(void* data);
void Shiversaw_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Shiversaw_EditorDraw(void);
void Shiversaw_EditorLoad(void);
#endif
void Shiversaw_Serialize(void);

// Extra Entity Functions
bool32 Shiversaw_CheckSawHit(EntityPlayer *player, int32 sawID);
void Shiversaw_CheckPlayerCollisions(void);
void Shiversaw_Unknown3(void);
void Shiversaw_Unknown4(void);
void Shiversaw_Unknown5(EntityPlayer *player);

void Shiversaw_State_Entry(void);
void Shiversaw_State_SetupBounds(void);
void Shiversaw_State_Unknown1(void);
void Shiversaw_Unknown9(void);
void Shiversaw_State_Unknown2(void);
void Shiversaw_State_Unknown3(void);
void Shiversaw_State_HitRecoil(void);
void Shiversaw_State_Destroyed(void);
void Shiversaw_Unknown10(void);
void Shiversaw_Unknown11(void);
void Shiversaw_Unknown12(void);
void Shiversaw_Unknown13(void);

void Shiversaw_CheckCrateCollisions(void);
void Shiversaw_Unknown14(int32 speed);
void Shiversaw_StateSaw_Unknown2(void);
void Shiversaw_StateSaw_Unknown3(void);
void Shiversaw_StateSaw_Unknown4(void);
void Shiversaw_StateSaw_Unknown5(void);
void Shiversaw_StateSaw_Destroyed(void);
void Shiversaw_StateSaw_Unknown7(void);
void Shiversaw_StateSaw_Unknown1(void);
void Shiversaw_ProcessSawMovement(int32 sawID);

void Shiversaw_StateDust_Unknown(void);

#endif //!OBJ_SHIVERSAW_H
