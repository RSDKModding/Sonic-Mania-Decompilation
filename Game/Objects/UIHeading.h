#ifndef OBJ_UIHEADING_H
#define OBJ_UIHEADING_H

//Object Class
class UIHeading : Object {

};

//Entity Class
class EntityUIHeading : Entity {

};

//Entity Functions
void UIHeading_Update();
void UIHeading_EarlyUpdate();
void UIHeading_LateUpdate();
void UIHeading_Draw();
void UIHeading_Setup(void* subtype);
void UIHeading_StageLoad();
void UIHeading_GetAttributes();

#endif //!OBJ_UIHEADING_H
