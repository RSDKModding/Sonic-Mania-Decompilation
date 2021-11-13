#ifndef OBJ_HEAVYRIDER_H
#define OBJ_HEAVYRIDER_H

#include "SonicMania.h"

typedef enum {
    HEAVYRIDER_RIDER,
    HEAVYRIDER_PLANESWITCH,
    HEAVYRIDER_FORCECHARGE_LEFT,
    HEAVYRIDER_FORCECHARGE_RIGHT,
    HEAVYRIDER_4,
    HEAVYRIDER_5,
    HEAVYRIDER_PUFF,
    HEAVYRIDER_FIREBALL,
    HEAVYRIDER_JIMMY,
}HeavyRiderTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint8 field_3;
    uint8 field_4;
    Vector2 debrisPos;
    uint8 field_10;
    int32 field_14;
    uint8 field_18;
    uint8 health;
    uint8 invincibilityTimer;
    uint8 field_1B;
    uint8 playerTimers[4];
    int32 field_20;
    int32 field_24;
    uint8 chargeTimer;
#if RETRO_USE_PLUS
    Entity *outro;
#endif
    uint16 aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox4;
    Hitbox hitbox3;
    Hitbox hitbox5;
    Hitbox hitbox6;
    Hitbox hitbox7;
    Hitbox hitbox8;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxCharge;
    uint16 sfxLaunch;
    uint16 sfxSkid;
    uint16 sfxJump;
    uint16 sfxCheer;
    uint16 sfxBumper;
} ObjectHeavyRider;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    uint8 type;
    int32 flags;
    int32 size;
    bool32 affectPlayer;
    int32 timer;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
} EntityHeavyRider;

// Object Struct
extern ObjectHeavyRider *HeavyRider;

// Standard Entity Events
void HeavyRider_Update(void);
void HeavyRider_LateUpdate(void);
void HeavyRider_StaticUpdate(void);
void HeavyRider_Draw(void);
void HeavyRider_Create(void* data);
void HeavyRider_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HeavyRider_EditorDraw(void);
void HeavyRider_EditorLoad(void);
#endif
void HeavyRider_Serialize(void);

// Extra Entity Functions
void HeavyRider_SpawnDebris(int frame, uint8 drawOrder, int x, int y);
void HeavyRider_CheckObjectCollisions(void);
void HeavyRider_Explode(void);
void HeavyRider_Unknown4(void);
void HeavyRider_Unknown5(void);
void HeavyRider_Unknown6(void);

void HeavyRider_State_SetupArena(void);
void HeavyRider_State_StartFight(void);
void HeavyRider_State_SetupRider(void);
void HeavyRider_State_Unknown1(void);
void HeavyRider_State_Unknown2(void);
void HeavyRider_State_Unknown3(void);
void HeavyRider_State_Charge(void);
void HeavyRider_State_Unknown4(void);
void HeavyRider_State_Destroyed(void);
void HeavyRider_State_Finish(void);
void HeavyRider_StateDraw_Unknown(void);

void HeavyRider_State_PlaneSwitch(void);
void HeavyRider_StateDraw1_Unknown(void);

void HeavyRider_State_ChargeTrigger(void);

void HeavyRider_StateDraw_Simple(void);

void HeavyRider_State_Fireball(void);

void HeavyRider_State_Puff(void);

void HeavyRider_StateJimmy_Idle(void);
void HeavyRider_StateJimmy_RevRelease(void);
void HeavyRider_StateJimmy_RevUp(void);

#endif //!OBJ_HEAVYRIDER_H
