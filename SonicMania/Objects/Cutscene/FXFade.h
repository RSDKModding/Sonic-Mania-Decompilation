#ifndef OBJ_FXFADE_H
#define OBJ_FXFADE_H

#include "Game.h"

// Object Class
struct ObjectFXFade {
    RSDK_OBJECT
};

// Entity Class
struct EntityFXFade {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 speedIn;
    int32 wait;
    int32 speedOut;
    uint32 color;
    bool32 oneWay;
    bool32 eventOnly;
    bool32 overHUD;
    bool32 fadeOutBlack;
#if MANIA_USE_PLUS
    bool32 transitionScene;
#endif
};

// Object Struct
extern ObjectFXFade *FXFade;

// Standard Entity Events
void FXFade_Update(void);
void FXFade_LateUpdate(void);
void FXFade_StaticUpdate(void);
void FXFade_Draw(void);
void FXFade_Create(void *data);
void FXFade_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FXFade_EditorDraw(void);
void FXFade_EditorLoad(void);
#endif
void FXFade_Serialize(void);

// Extra Entity Functions
void FXFade_StopAll(void);
void FXFade_State_FadeOut(void);
void FXFade_State_Wait(void);
void FXFade_State_FadeIn(void);
void FXFade_State_FadeInBlack(void);

#endif //! OBJ_FXFADE_H
