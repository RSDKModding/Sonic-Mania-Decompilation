#ifndef OBJ_UIRANKBUTTON_H
#define OBJ_UIRANKBUTTON_H

//Object Class
class UIRankButton : Object {

};

//Entity Class
class EntityUIRankButton : Entity {

};

//Entity Functions
void UIRankButton_Update();
void UIRankButton_EarlyUpdate();
void UIRankButton_LateUpdate();
void UIRankButton_Draw();
void UIRankButton_Setup(void* subtype);
void UIRankButton_StageLoad();
void UIRankButton_GetAttributes();

#endif //!OBJ_UIRANKBUTTON_H
