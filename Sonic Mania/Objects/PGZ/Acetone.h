#ifndef OBJ_ACETONE_H
#define OBJ_ACETONE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxFrostThrower;
    uint16 sfxFreeze;
} ObjectAcetone;

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
} EntityAcetone;

// Object Entity
extern ObjectAcetone *Acetone;

// Standard Entity Events
void Acetone_Update(void);
void Acetone_LateUpdate(void);
void Acetone_StaticUpdate(void);
void Acetone_Draw(void);
void Acetone_Create(void* data);
void Acetone_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Acetone_EditorDraw(void);
void Acetone_EditorLoad(void);
#endif
void Acetone_Serialize(void);

// Extra Entity Functions
void Acetone_DrawGustFX(void);
void Acetone_CheckPlayerCollisions(void);
void Acetone_Unknown3(void);
void Acetone_Unknown4(void);
void Acetone_State_IntervalWait(void);
void Acetone_Unknown6(void);
void Acetone_Unknown7(void);


#endif //!OBJ_ACETONE_H
