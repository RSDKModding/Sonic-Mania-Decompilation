#ifndef OBJ_UIPOPOVER_H
#define OBJ_UIPOPOVER_H

//Object Class
class UIPopover : Object {

};

//Entity Class
class EntityUIPopover : Entity {

};

//Entity Functions
void UIPopover_Update();
void UIPopover_EarlyUpdate();
void UIPopover_LateUpdate();
void UIPopover_Draw();
void UIPopover_Setup(void* subtype);
void UIPopover_StageLoad();
void UIPopover_GetAttributes();

#endif //!OBJ_UIPOPOVER_H
