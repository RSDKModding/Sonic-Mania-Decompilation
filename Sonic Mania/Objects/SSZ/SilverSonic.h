#ifndef OBJ_SILVERSONIC_H
#define OBJ_SILVERSONIC_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxJump;
    uint16 sfxDash;
    uint16 sfxBoost;
    uint16 sfxRebound;
    uint16 sfxArm;
} ObjectSilverSonic;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 field_60;
    int32 field_64;
    int32 invincibilityTimer;
    Hitbox *hitboxPtr;
    Hitbox *hitboxBadnikPtr;
    Animator animator;
} EntitySilverSonic;

// Object Struct
extern ObjectSilverSonic *SilverSonic;

// Standard Entity Events
void SilverSonic_Update(void);
void SilverSonic_LateUpdate(void);
void SilverSonic_StaticUpdate(void);
void SilverSonic_Draw(void);
void SilverSonic_Create(void* data);
void SilverSonic_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SilverSonic_EditorDraw(void);
void SilverSonic_EditorLoad(void);
#endif
void SilverSonic_Serialize(void);

// Extra Entity Functions
void SilverSonic_HandleNextAttack(void);

void SilverSonic_CheckPlayerCollisions_Badnik(void);
void SilverSonic_CheckPlayerCollisions_Ball(void);
void SilverSonic_CheckPlayerCollisions_Arm(void);

void SilverSonic_State_Unknown1(void);
void SilverSonic_State2_Unknown5(void);
void SilverSonic_State_Unknown2(void);
void SilverSonic_State2_Unknown1(void);
void SilverSonic_State2_Unknown2(void);
void SilverSonic_State2_Unknown3(void);
void SilverSonic_State2_Unknown4(void);
void SilverSonic_State2_Unknown6(void);
void SilverSonic_State_Unknown3(void);
void SilverSonic_State_Unknown4(void);
void SilverSonic_State_Unknown5(void);
void SilverSonic_State_Unknown6(void);
void SilverSonic_State2_Explode(void);

#endif //!OBJ_SILVERSONIC_H
