#ifndef OBJ_UIWAITSPINNER_H
#define OBJ_UIWAITSPINNER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    int32 timer;
    Entity *activeSpinner;
} ObjectUIWaitSpinner;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    bool32 flag;
    Animator animator;
} EntityUIWaitSpinner;

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
void UIWaitSpinner_Wait(void);
void UIWaitSpinner_Wait2(void);
void UIWaitSpinner_State_Wait(void);
void UIWaitSpinner_State_Wait2(void);

#endif //!OBJ_UIWAITSPINNER_H
