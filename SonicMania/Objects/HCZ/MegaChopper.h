#ifndef OBJ_MEGACHOPPER_H
#define OBJ_MEGACHOPPER_H

#include "Game.h"

// Object Class
struct ObjectMegaChopper {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxChop;
    uint16 aniFrames;
};

// Entity Class
struct EntityMegaChopper {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    EntityPlayer *grabbedPlayer;
    Vector2 playerPos;
    uint8 playerDir;
    uint16 shakeTimer;
    uint16 shakeCount;
    uint16 lastShakeFlags;
    uint8 nibbleTimer;
    Animator animator;
};

// Object Struct
extern ObjectMegaChopper *MegaChopper;

// Standard Entity Events
void MegaChopper_Update(void);
void MegaChopper_LateUpdate(void);
void MegaChopper_StaticUpdate(void);
void MegaChopper_Draw(void);
void MegaChopper_Create(void *data);
void MegaChopper_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MegaChopper_EditorDraw(void);
void MegaChopper_EditorLoad(void);
#endif
void MegaChopper_Serialize(void);

// Extra Entity Functions
void MegaChopper_DebugSpawn(void);
void MegaChopper_DebugDraw(void);

void MegaChopper_CheckPlayerCollisions(void);
void MegaChopper_CheckOffScreen(void);

void MegaChopper_Input_GrabbedP1(void);
void MegaChopper_Input_GrabbedP2(void);
void MegaChopper_Input_GrabbedP2_AI(void);

void MegaChopper_State_Init(void);
void MegaChopper_State_InWater(void);
void MegaChopper_State_OutOfWater(void);
void MegaChopper_State_Chopping(void);
void MegaChopper_State_ShakenOff(void);

#endif //! OBJ_MEGACHOPPER_H
