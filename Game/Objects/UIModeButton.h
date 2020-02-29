#ifndef OBJ_UIMODEBUTTON_H
#define OBJ_UIMODEBUTTON_H

//Object Class
class UIModeButton : Object {

};

//Entity Class
class EntityUIModeButton : Entity {

};

//Entity Functions
void UIModeButton_Update();
void UIModeButton_EarlyUpdate();
void UIModeButton_LateUpdate();
void UIModeButton_Draw();
void UIModeButton_Setup(void* subtype);
void UIModeButton_StageLoad();
void UIModeButton_GetAttributes();

#endif //!OBJ_UIMODEBUTTON_H
