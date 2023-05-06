#ifndef OBJ_TRYAGAINE_H
#define OBJ_TRYAGAINE_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectTryAgainE {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 playerFrames;
};

// Entity Class
struct EntityTryAgainE {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 unused1;
    int32 unused2;
    Animator mainAnimator;
    Animator handUpAnimator;
    Animator handDownAnimator;
    Animator player1Animator;
    Animator player2Animator;
};

// Object Struct
extern ObjectTryAgainE *TryAgainE;

// Standard Entity Events
void TryAgainE_Update(void);
void TryAgainE_LateUpdate(void);
void TryAgainE_StaticUpdate(void);
void TryAgainE_Draw(void);
void TryAgainE_Create(void *data);
void TryAgainE_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TryAgainE_EditorDraw(void);
void TryAgainE_EditorLoad(void);
#endif
void TryAgainE_Serialize(void);

// Extra Entity Functions
void TryAgainE_SetupEmeralds(void);
void TryAgainE_State_Stinger(void);
#endif

#endif //! OBJ_TRYAGAINE_H
