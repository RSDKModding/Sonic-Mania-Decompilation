#ifndef OBJ_UIMEDALLIONPANEL_H
#define OBJ_UIMEDALLIONPANEL_H

//Object Class
class UIMedallionPanel : Object {

};

//Entity Class
class EntityUIMedallionPanel : Entity {

};

//Entity Functions
void UIMedallionPanel_Update();
void UIMedallionPanel_EarlyUpdate();
void UIMedallionPanel_LateUpdate();
void UIMedallionPanel_Draw();
void UIMedallionPanel_Setup(void* subtype);
void UIMedallionPanel_StageLoad();
void UIMedallionPanel_GetAttributes();

#endif //!OBJ_UIMEDALLIONPANEL_H
