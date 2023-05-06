#ifndef OBJ_TITLELOGO_H
#define OBJ_TITLELOGO_H

#include "Game.h"

typedef enum {
    TITLELOGO_EMBLEM,
    TITLELOGO_RIBBON,
    TITLELOGO_GAMETITLE,
    TITLELOGO_POWERLED,
    TITLELOGO_COPYRIGHT,
    TITLELOGO_RINGBOTTOM,
    TITLELOGO_PRESSSTART,
#if MANIA_USE_PLUS
    TITLELOGO_PLUS,
#endif
} TitleLogoTypes;

// Object Class
struct ObjectTitleLogo {
    RSDK_OBJECT
    uint16 aniFrames;
#if MANIA_USE_PLUS
    uint16 plusFrames;
    uint16 sfxPlus;
#endif
};

// Entity Class
struct EntityTitleLogo {
    RSDK_ENTITY
    int32 type;
#if MANIA_USE_PLUS
    StateMachine(state);
#endif
    bool32 showRibbonCenter;
    int32 timer;
    int32 storeY;
    Animator mainAnimator;
#if MANIA_USE_PLUS
    Animator plusAnimator;
#endif
    Animator ribbonCenterAnimator;
};

// Object Struct
extern ObjectTitleLogo *TitleLogo;

// Standard Entity Events
void TitleLogo_Update(void);
void TitleLogo_LateUpdate(void);
void TitleLogo_StaticUpdate(void);
void TitleLogo_Draw(void);
void TitleLogo_Create(void *data);
void TitleLogo_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TitleLogo_EditorDraw(void);
void TitleLogo_EditorLoad(void);
#endif
void TitleLogo_Serialize(void);

// Extra Entity Functions
void TitleLogo_SetupPressStart(void);
#if MANIA_USE_PLUS
void TitleLogo_State_Ribbon(void);
void TitleLogo_State_PressButton(void);
void TitleLogo_State_HandleSetup(void);
void TitleLogo_State_PlusLogo(void);
void TitleLogo_State_PlusShine(void);
#endif

#endif //! OBJ_TITLELOGO_H
