#ifndef OBJ_TRYAGAIN_H
#define OBJ_TRYAGAIN_H

#include "Game.h"

// Object Class
struct ObjectTryAgain {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityTryAgain {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 unused1;
    int32 unused2;
    Vector2 eggmanPos;
    int32 unused3;
    int32 eggmanVelocityY;
    Vector2 rubyPos;
    Vector2 rubyVelocity;
    Animator debrisAnimator;
    Animator eggmanAnimator;
    Animator rubyAnimator;
};

// Object Struct
extern ObjectTryAgain *TryAgain;

// Standard Entity Events
void TryAgain_Update(void);
void TryAgain_LateUpdate(void);
void TryAgain_StaticUpdate(void);
void TryAgain_Draw(void);
void TryAgain_Create(void *data);
void TryAgain_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TryAgain_EditorDraw(void);
void TryAgain_EditorLoad(void);
#endif
void TryAgain_Serialize(void);

// Extra Entity Functions
void TryAgain_State_Init(void);
void TryAgain_State_EnterEggman(void);
void TryAgain_State_EggmanLaugh(void);
void TryAgain_State_Stinger(void);

#endif //! OBJ_TRYAGAIN_H
