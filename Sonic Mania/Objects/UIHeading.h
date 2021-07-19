#ifndef OBJ_UIHEADING_H
#define OBJ_UIHEADING_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectUIHeading;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int field_5C;
    Vector2 startPos;
    int headingID;
    Animator data2;
    Animator animator;
    ushort spriteIndex;
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
void UIHeading_EditorDraw(void);
void UIHeading_EditorLoad(void);
void UIHeading_Serialize(void);

// Extra Entity Functions
void UIHeading_LoadSprites(void);

#endif //!OBJ_UIHEADING_H
