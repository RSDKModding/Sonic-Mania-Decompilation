#ifndef OBJ_PSZEGGMAN_H
#define OBJ_PSZEGGMAN_H

#include "Game.h"
#include "ERZ/PhantomRuby.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 controlFrames;
    uint16 aniFrames;
} ObjectPSZEggman;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Vector2 eggmanPos;
    EntityPhantomRuby *ruby;
    Animator eggmanAnimator;
    Animator terminalAnimator;
    Animator displayAnimator;
    Animator scanlinesAnimator;
} EntityPSZEggman;

// Object Struct
extern ObjectPSZEggman *PSZEggman;

// Standard Entity Events
void PSZEggman_Update(void);
void PSZEggman_LateUpdate(void);
void PSZEggman_StaticUpdate(void);
void PSZEggman_Draw(void);
void PSZEggman_Create(void *data);
void PSZEggman_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PSZEggman_EditorDraw(void);
void PSZEggman_EditorLoad(void);
#endif
void PSZEggman_Serialize(void);

// Extra Entity Functions
void PSZEggman_State_TurnRound(void);

#endif //! OBJ_PSZEGGMAN_H
