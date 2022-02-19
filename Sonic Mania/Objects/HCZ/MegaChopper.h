#ifndef OBJ_MEGACHOPPER_H
#define OBJ_MEGACHOPPER_H

#include "SonicMania.h"

// Object Class
struct ObjectMegaChopper {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
};

// Entity Class
struct EntityMegaChopper {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    Entity *playerPtr;
    Vector2 playerPos;
    uint8 playerDir;
    uint16 field_72;
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
void MegaChopper_Create(void* data);
void MegaChopper_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MegaChopper_EditorDraw(void);
void MegaChopper_EditorLoad(void);
#endif
void MegaChopper_Serialize(void);

// Extra Entity Functions
void MegaChopper_DebugSpawn(void);
void MegaChopper_DebugDraw(void);

void MegaChopper_CheckPlayerCollisions(void);
void MegaChopper_CheckOffScreen(void);

void MegaChopper_PlayerInput_StateP1(void);
void MegaChopper_PlayerInput_StateP2(void);
void MegaChopper_PlayerInput_StateP2_AI(void);

void MegaChopper_State_Setup(void);
void MegaChopper_State_Unknown1(void);
void MegaChopper_State_Unknown2(void);
void MegaChopper_State_Unknown3(void);
void MegaChopper_State_Unknown4(void);

#endif //!OBJ_MEGACHOPPER_H
