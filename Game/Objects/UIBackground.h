#ifndef OBJ_UIBACKGROUND_H
#define OBJ_UIBACKGROUND_H

//Object Class
class UIBackground : Object {

};

//Entity Class
class EntityUIBackground : Entity {

};

//Entity Functions
void UIBackground_Update();
void UIBackground_EarlyUpdate();
void UIBackground_LateUpdate();
void UIBackground_Draw();
void UIBackground_Setup(void* subtype);
void UIBackground_StageLoad();
void UIBackground_GetAttributes();

#endif //!OBJ_UIBACKGROUND_H
