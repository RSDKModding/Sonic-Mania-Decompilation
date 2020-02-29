#ifndef OBJ_UIOPTIONPANEL_H
#define OBJ_UIOPTIONPANEL_H

//Object Class
class UIOptionPanel : Object {

};

//Entity Class
class EntityUIOptionPanel : Entity {

};

//Entity Functions
void UIOptionPanel_Update();
void UIOptionPanel_EarlyUpdate();
void UIOptionPanel_LateUpdate();
void UIOptionPanel_Draw();
void UIOptionPanel_Setup(void* subtype);
void UIOptionPanel_StageLoad();
void UIOptionPanel_GetAttributes();

#endif //!OBJ_UIOPTIONPANEL_H
