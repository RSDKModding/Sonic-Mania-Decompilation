#ifndef OBJ_TMZALERT_H
#define OBJ_TMZALERT_H

#include "Game.h"

// Object Class
struct ObjectTMZAlert {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityTMZAlert {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Animator screenAnimator;
    Animator messageAnimator;
};

// Object Struct
extern ObjectTMZAlert *TMZAlert;

// Standard Entity Events
void TMZAlert_Update(void);
void TMZAlert_LateUpdate(void);
void TMZAlert_StaticUpdate(void);
void TMZAlert_Draw(void);
void TMZAlert_Create(void *data);
void TMZAlert_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TMZAlert_EditorDraw(void);
void TMZAlert_EditorLoad(void);
#endif
void TMZAlert_Serialize(void);

// Extra Entity Functions
void TMZAlert_State_Activating(void);
void TMZAlert_State_Alerting(void);
void TMZAlert_State_ShuttingDown(void);

#endif //! OBJ_TMZALERT_H
