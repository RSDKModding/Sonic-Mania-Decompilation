#ifndef OBJ_MICDROP_H
#define OBJ_MICDROP_H

#include "Game.h"

// Object Class
struct ObjectMicDrop {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxRange;
    uint16 aniFrames;
    uint16 sfxElectrify;
};

// Entity Class
struct EntityMicDrop {
    RSDK_ENTITY
    StateMachine(state);
    Animator bodyAnimator;
    Animator wireAnimator;
    Vector2 startPos;
    uint16 distance;
    int32 timer;
    int32 swingSpeed;
    int32 swingVel;
    int32 radius;
    int32 swingPos;
};

// Object Struct
extern ObjectMicDrop *MicDrop;

// Standard Entity Events
void MicDrop_Update(void);
void MicDrop_LateUpdate(void);
void MicDrop_StaticUpdate(void);
void MicDrop_Draw(void);
void MicDrop_Create(void *data);
void MicDrop_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MicDrop_EditorDraw(void);
void MicDrop_EditorLoad(void);
#endif
void MicDrop_Serialize(void);

// Extra Entity Functions
void MicDrop_DebugSpawn(void);
void MicDrop_DebugDraw(void);

void MicDrop_CheckPlayerCollisions(void);
void MicDrop_CheckOffScreen(void);

void MicDrop_State_Init(void);
void MicDrop_State_CheckForPlayer(void);
void MicDrop_State_DropDown(void);
void MicDrop_State_DropRecoil(void);
void MicDrop_State_Idle(void);
void MicDrop_State_Swinging(void);
void MicDrop_State_Electrify(void);

#endif //! OBJ_MICDROP_H
