#ifndef OBJ_FXWAVERING_H
#define OBJ_FXWAVERING_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectFXWaveRing {
    RSDK_OBJECT
};

// Entity Class
struct EntityFXWaveRing {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 angleVel;
    int32 angleVel2;
    int32 angle2;
    int32 angleInc;
    int32 shrinkSpeed;
    uint8 r;
    uint8 g;
    uint8 b;
    int32 radiusOffset;
    Entity *parent;
    Vector2 offsetPos;
    int32 pointCount;
    int32 radius;
};

// Object Struct
extern ObjectFXWaveRing *FXWaveRing;

// Standard Entity Events
void FXWaveRing_Update(void);
void FXWaveRing_LateUpdate(void);
void FXWaveRing_StaticUpdate(void);
void FXWaveRing_Draw(void);
void FXWaveRing_Create(void *data);
void FXWaveRing_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FXWaveRing_EditorDraw(void);
void FXWaveRing_EditorLoad(void);
#endif
void FXWaveRing_Serialize(void);

// Extra Entity Functions
void FXWaveRing_State_FadeIn(void);
void FXWaveRing_State_Wait(void);
void FXWaveRing_State_FadeOut(void);
#endif

#endif //! OBJ_FXWAVERING_H
