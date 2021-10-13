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
    bool32 flag;
    int32 field_A4[4];
    int32 field_B4[4];
    Vector2 field_C4[12];
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
void Acetone_EditorDraw(void);
void Acetone_EditorLoad(void);
void Acetone_Serialize(void);

// Extra Entity Functions
void Acetone_Unknown1(void);
void Acetone_Unknown2(void);
void Acetone_Unknown3(void);
void Acetone_Unknown4(void);
void Acetone_Unknown5(void);
void Acetone_Unknown6(void);
void Acetone_Unknown7(void);


#endif //!OBJ_ACETONE_H
