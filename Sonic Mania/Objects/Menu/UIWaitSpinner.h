#ifndef OBJ_UIWAITSPINNER_H
#define OBJ_UIWAITSPINNER_H

#include "SonicMania.h"

// Object Class
struct ObjectUIWaitSpinner {
    RSDK_OBJECT
    uint16 aniFrames;
    int32 timer;
    Entity *activeSpinner;
};

// Entity Class
struct EntityUIWaitSpinner {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    bool32 fadedIn;
    Animator animator;
};

// Object Struct
extern ObjectUIWaitSpinner *UIWaitSpinner;

// Standard Entity Events
void UIWaitSpinner_Update(void);
void UIWaitSpinner_LateUpdate(void);
void UIWaitSpinner_StaticUpdate(void);
void UIWaitSpinner_Draw(void);
void UIWaitSpinner_Create(void* data);
void UIWaitSpinner_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIWaitSpinner_EditorDraw(void);
void UIWaitSpinner_EditorLoad(void);
#endif
void UIWaitSpinner_Serialize(void);

// Extra Entity Functions
void UIWaitSpinner_StartWait(void);
void UIWaitSpinner_FinishWait(void);
void UIWaitSpinner_State_Wait(void);
void UIWaitSpinner_State_WaitAndDestroy(void);

#endif //!OBJ_UIWAITSPINNER_H
