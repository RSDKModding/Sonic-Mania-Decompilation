#ifndef OBJ_UIWAITSPINNER_H
#define OBJ_UIWAITSPINNER_H

//Object Class
class UIWaitSpinner : Object {

};

//Entity Class
class EntityUIWaitSpinner : Entity {

};

//Entity Functions
void UIWaitSpinner_Update();
void UIWaitSpinner_EarlyUpdate();
void UIWaitSpinner_LateUpdate();
void UIWaitSpinner_Draw();
void UIWaitSpinner_Setup(void* subtype);
void UIWaitSpinner_StageLoad();
void UIWaitSpinner_GetAttributes();

#endif //!OBJ_UIWAITSPINNER_H
