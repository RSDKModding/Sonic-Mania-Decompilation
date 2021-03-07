#ifndef OBJ_UIHEADING_H
#define OBJ_UIHEADING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIHeading : Object {

};

// Entity Class
struct EntityUIHeading : Entity {

};

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


#endif //!OBJ_UIHEADING_H
