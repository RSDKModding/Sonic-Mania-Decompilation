#ifndef OBJ_UIVSZONEBUTTON_H
#define OBJ_UIVSZONEBUTTON_H

//Object Class
class UIVsZoneButton : Object {

};

//Entity Class
class EntityUIVsZoneButton : Entity {

};

//Entity Functions
void UIVsZoneButton_Update();
void UIVsZoneButton_EarlyUpdate();
void UIVsZoneButton_LateUpdate();
void UIVsZoneButton_Draw();
void UIVsZoneButton_Setup(void* subtype);
void UIVsZoneButton_StageLoad();
void UIVsZoneButton_GetAttributes();

#endif //!OBJ_UIVSZONEBUTTON_H
