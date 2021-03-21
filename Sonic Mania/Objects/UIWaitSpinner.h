#ifndef OBJ_UIWAITSPINNER_H
#define OBJ_UIWAITSPINNER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    int timer;
    Entity *activeSpinner;
} ObjectUIWaitSpinner;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    int timer;
    bool32 flag;
    AnimationData data;
} EntityUIWaitSpinner;

// Object Struct
extern ObjectUIWaitSpinner *UIWaitSpinner;

// Standard Entity Events
void UIWaitSpinner_Update();
void UIWaitSpinner_LateUpdate();
void UIWaitSpinner_StaticUpdate();
void UIWaitSpinner_Draw();
void UIWaitSpinner_Create(void* data);
void UIWaitSpinner_StageLoad();
void UIWaitSpinner_EditorDraw();
void UIWaitSpinner_EditorLoad();
void UIWaitSpinner_Serialize();

// Extra Entity Functions
void UIWaitSpinner_Wait();
void UIWaitSpinner_WaitReplay();
void UIWaitSpinner_State_Wait();
void UIWaitSpinner_State_Wait2();

#endif //!OBJ_UIWAITSPINNER_H
