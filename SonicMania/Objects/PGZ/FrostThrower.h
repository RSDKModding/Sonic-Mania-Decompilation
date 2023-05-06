#ifndef OBJ_FROSTTHROWER_H
#define OBJ_FROSTTHROWER_H

#include "Game.h"

// Object Class
struct ObjectFrostThrower {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxFrostThrower;
    uint16 sfxFreeze;
};

// Entity Class
struct EntityFrostThrower {
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

// Object Struct
extern ObjectFrostThrower *FrostThrower;

// Standard Entity Events
void FrostThrower_Update(void);
void FrostThrower_LateUpdate(void);
void FrostThrower_StaticUpdate(void);
void FrostThrower_Draw(void);
void FrostThrower_Create(void *data);
void FrostThrower_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FrostThrower_EditorDraw(void);
void FrostThrower_EditorLoad(void);
#endif
void FrostThrower_Serialize(void);

// Extra Entity Functions
void FrostThrower_DrawGustFX(void);
void FrostThrower_CheckPlayerCollisions(void);
void FrostThrower_HandleGustCount(void);
void FrostThrower_HandleGustPos(void);
void FrostThrower_State_AwaitInterval(void);
void FrostThrower_State_Dispensing(void);
void FrostThrower_State_StopDispensing(void);

#endif //! OBJ_FROSTTHROWER_H
