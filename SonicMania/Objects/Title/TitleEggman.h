#ifndef OBJ_TITLEEGGMAN_H
#define OBJ_TITLEEGGMAN_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectTitleEggman {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityTitleEggman {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Vector2 originPos;
    Animator smokeAnimator;
    Animator eggmanAnimator;
    Animator chainAnimator;
    Animator capsuleAnimator;
};

// Object Struct
extern ObjectTitleEggman *TitleEggman;

// Standard Entity Events
void TitleEggman_Update(void);
void TitleEggman_LateUpdate(void);
void TitleEggman_StaticUpdate(void);
void TitleEggman_Draw(void);
void TitleEggman_Create(void *data);
void TitleEggman_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TitleEggman_EditorDraw(void);
void TitleEggman_EditorLoad(void);
#endif
void TitleEggman_Serialize(void);

// Extra Entity Functions
void TitleEggman_State_Dust(void);
void TitleEggman_State_Wait(void);
void TitleEggman_State_Move(void);
#endif

#endif //! OBJ_TITLEEGGMAN_H
