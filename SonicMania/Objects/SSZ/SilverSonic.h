#ifndef OBJ_SILVERSONIC_H
#define OBJ_SILVERSONIC_H

#include "Game.h"

typedef enum {
    SSONIC_ANI_IDLE,
    SSONIC_ANI_CROUCH,
    SSONIC_ANI_BOOSTREADY,
    SSONIC_ANI_BOOST,
    SSONIC_ANI_ARMATTACK,
    SSONIC_ANI_JUMP,
    SSONIC_ANI_SPINDASH,
} SilverSonicAniIDs;

typedef enum {
    SSONIC_ATTACK_BOOST,
    SSONIC_ATTACK_ROLL,
} SilverSonicAttackIDs;

// Object Class
struct ObjectSilverSonic {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxJump;
    uint16 sfxDash;
    uint16 sfxBoost;
    uint16 sfxRebound;
    uint16 sfxArm;
};

// Entity Class
struct EntitySilverSonic {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 attackType;
    int32 attackRepeatCount;
    int32 invincibilityTimer;
    Hitbox *outerBox;
    Hitbox *innerBox;
    Animator animator;
};

// Object Struct
extern ObjectSilverSonic *SilverSonic;

// Standard Entity Events
void SilverSonic_Update(void);
void SilverSonic_LateUpdate(void);
void SilverSonic_StaticUpdate(void);
void SilverSonic_Draw(void);
void SilverSonic_Create(void *data);
void SilverSonic_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SilverSonic_EditorDraw(void);
void SilverSonic_EditorLoad(void);
#endif
void SilverSonic_Serialize(void);

// Extra Entity Functions
void SilverSonic_HandleNextAttack(void);

void SilverSonic_CheckPlayerCollisions_Badnik(void);
void SilverSonic_CheckPlayerCollisions_Ball(void);
void SilverSonic_CheckPlayerCollisions_Arm(void);

void SilverSonic_State_Appear(void);
void SilverSonic_State_FinishedAttack(void);
void SilverSonic_State_ArmAttack(void);
void SilverSonic_State_Crouch(void);
void SilverSonic_State_Spindash(void);
void SilverSonic_State_Roll(void);
void SilverSonic_State_RollJump(void);
void SilverSonic_State_RollRebound(void);
void SilverSonic_State_BoostReady(void);
void SilverSonic_State_Boost_Air(void);
void SilverSonic_State_Boost_Ground(void);
void SilverSonic_State_FinishedBoost(void);
void SilverSonic_State_Explode(void);

#endif //! OBJ_SILVERSONIC_H
