#ifndef OBJ_ACETONE_H
#define OBJ_ACETONE_H

#include "Game.h"

// Object Class
struct ObjectAcetone {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxFrostThrower;
    uint16 sfxFreeze;
};

// Entity Class
struct EntityAcetone {
    RSDK_ENTITY
    StateMachine(state);
    uint16 timer;
    Animator dispenseAnimator;
    Animator gustAnimator;
    uint16 interval;
    uint16 intervalOffset;
    uint16 duration;
    Hitbox hitbox;
    bool32 isActive;
    int32 maxGustCount[4];
    int32 gustCount[4];
    Vector2 gustPos[12];
};

// Object Entity
extern ObjectAcetone *Acetone;

// Standard Entity Events
void Acetone_Update(void);
void Acetone_LateUpdate(void);
void Acetone_StaticUpdate(void);
void Acetone_Draw(void);
void Acetone_Create(void *data);
void Acetone_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Acetone_EditorDraw(void);
void Acetone_EditorLoad(void);
#endif
void Acetone_Serialize(void);

// Extra Entity Functions
void Acetone_DrawGustFX(void);
void Acetone_CheckPlayerCollisions(void);
void Acetone_HandleGustCount(void);
void Acetone_HandleGustPos(void);
void Acetone_State_AwaitInterval(void);
void Acetone_State_Dispensing(void);
void Acetone_State_StopDispensing(void);

#endif //! OBJ_ACETONE_H
