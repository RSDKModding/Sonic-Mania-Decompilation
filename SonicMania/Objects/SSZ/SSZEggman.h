#ifndef OBJ_SSZEGGMAN_H
#define OBJ_SSZEGGMAN_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectSSZEggman {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntitySSZEggman {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Vector2 originPos;
    Vector2 offset;
    int32 speed;
    EntityPhantomRuby *ruby;
    Animator eggmanAnimator;
    Animator seatAnimator;
    Animator mobileAnimator;
    Animator panelAnimator;
};

// Object Struct
extern ObjectSSZEggman *SSZEggman;

// Standard Entity Events
void SSZEggman_Update(void);
void SSZEggman_LateUpdate(void);
void SSZEggman_StaticUpdate(void);
void SSZEggman_Draw(void);
void SSZEggman_Create(void *data);
void SSZEggman_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SSZEggman_EditorDraw(void);
void SSZEggman_EditorLoad(void);
#endif
void SSZEggman_Serialize(void);

// Extra Entity Functions
void SSZEggman_State_Init(void);
void SSZEggman_State_HoldingRuby(void);
void SSZEggman_State_ThrownRuby(void);
void SSZEggman_State_WatchMetalTransform(void);
void SSZEggman_State_FlyAway(void);
#endif

#endif //! OBJ_SSZEGGMAN_H
