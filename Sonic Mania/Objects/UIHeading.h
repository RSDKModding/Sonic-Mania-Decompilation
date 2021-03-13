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
    void (*state)();
    int field_5C;
    Vector2 startPos;
    int headingID;
    EntityAnimationData data2;
    EntityAnimationData data;
    ushort spriteIndex;
} EntityUIHeading;

// Object Struct
extern ObjectUIHeading *UIHeading;

// Standard Entity Events
void UIHeading_Update();
void UIHeading_LateUpdate();
void UIHeading_StaticUpdate();
void UIHeading_Draw();
void UIHeading_Create(void* data);
void UIHeading_StageLoad();
void UIHeading_EditorDraw();
void UIHeading_EditorLoad();
void UIHeading_Serialize();

// Extra Entity Functions
void UIHeading_LoadSprites();

#endif //!OBJ_UIHEADING_H
