#ifndef OBJ_FROSTTHROWER_H
#define OBJ_FROSTTHROWER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxFrostThrower;
    uint16 sfxFreeze;
} ObjectFrostThrower;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint16 timer;
    Animator animator1;
    Animator animator2;
    uint16 interval;
    uint16 intervalOffset;
    uint16 duration;
    Hitbox hitbox;
    bool32 isActive;
    int32 field_A4[4];
    int32 field_B4[4];
    Vector2 gustPos[12];
} EntityFrostThrower;

// Object Struct
extern ObjectFrostThrower *FrostThrower;

// Standard Entity Events
void FrostThrower_Update(void);
void FrostThrower_LateUpdate(void);
void FrostThrower_StaticUpdate(void);
void FrostThrower_Draw(void);
void FrostThrower_Create(void* data);
void FrostThrower_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void FrostThrower_EditorDraw(void);
void FrostThrower_EditorLoad(void);
#endif
void FrostThrower_Serialize(void);

// Extra Entity Functions
void FrostThrower_DrawGustFX(void);
void FrostThrower_CheckPlayerCollisions(void);
void FrostThrower_Unknown3(void);
void FrostThrower_Unknown4(void);
void FrostThrower_State_IntervalWait(void);
void FrostThrower_Unknown6(void);
void FrostThrower_Unknown7(void);

#endif //!OBJ_FROSTTHROWER_H
