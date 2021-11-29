#ifndef OBJ_UIHEADING_H
#define OBJ_UIHEADING_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 textFrames;
} ObjectUIHeading;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused;
    Vector2 startPos;
    int32 headingID;
    Animator headingAnimator;
    Animator textAnimator;
    uint16 textFrames;
} EntityUIHeading;

// Object Struct
extern ObjectUIHeading *UIHeading;

// Standard Entity Events
void UIHeading_Update(void);
void UIHeading_LateUpdate(void);
void UIHeading_StaticUpdate(void);
void UIHeading_Draw(void);
void UIHeading_Create(void* data);
void UIHeading_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIHeading_EditorDraw(void);
void UIHeading_EditorLoad(void);
#endif
void UIHeading_Serialize(void);

// Extra Entity Functions
void UIHeading_LoadSprites(void);

#endif //!OBJ_UIHEADING_H
