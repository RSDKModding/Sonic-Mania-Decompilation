#ifndef OBJ_UIWINSIZE_H
#define OBJ_UIWINSIZE_H

//Object Class
class UIWinSize : Object {

};

//Entity Class
class EntityUIWinSize : Entity {

};

//Entity Functions
void UIWinSize_Update();
void UIWinSize_EarlyUpdate();
void UIWinSize_LateUpdate();
void UIWinSize_Draw();
void UIWinSize_Setup(void* subtype);
void UIWinSize_StageLoad();
void UIWinSize_GetAttributes();

#endif //!OBJ_UIWINSIZE_H
