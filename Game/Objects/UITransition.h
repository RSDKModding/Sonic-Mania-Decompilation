#ifndef OBJ_UITRANSITION_H
#define OBJ_UITRANSITION_H

//Object Class
class UITransition : Object {

};

//Entity Class
class EntityUITransition : Entity {

};

//Entity Functions
void UITransition_Update();
void UITransition_EarlyUpdate();
void UITransition_LateUpdate();
void UITransition_Draw();
void UITransition_Setup(void* subtype);
void UITransition_StageLoad();
void UITransition_GetAttributes();

#endif //!OBJ_UITRANSITION_H
