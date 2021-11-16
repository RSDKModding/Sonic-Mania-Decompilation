#ifndef OBJ_MICDROP_H
#define OBJ_MICDROP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
    uint16 sfxElectrify;
} ObjectMicDrop;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator1;
    Animator animator2;
    Vector2 startPos;
    uint16 distance;
    uint16 field_96;
    int32 timer;
    int32 field_9C;
    int32 field_A0;
    int32 radius;
    int32 field_A8;
} EntityMicDrop;

// Object Struct
extern ObjectMicDrop *MicDrop;

// Standard Entity Events
void MicDrop_Update(void);
void MicDrop_LateUpdate(void);
void MicDrop_StaticUpdate(void);
void MicDrop_Draw(void);
void MicDrop_Create(void* data);
void MicDrop_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MicDrop_EditorDraw(void);
void MicDrop_EditorLoad(void);
#endif
void MicDrop_Serialize(void);

// Extra Entity Functions
void MicDrop_DebugSpawn(void);
void MicDrop_DebugDraw(void);

void MicDrop_CheckPlayerCollisions(void);
void MicDrop_CheckOnScreen(void);

void MicDrop_State_Setup(void);
void MicDrop_State_Unknown1(void);
void MicDrop_State_Unknown2(void);
void MicDrop_State_Unknown3(void);
void MicDrop_State_Unknown4(void);
void MicDrop_State_Unknown5(void);
void MicDrop_State_Unknown6(void);

#endif //!OBJ_MICDROP_H
