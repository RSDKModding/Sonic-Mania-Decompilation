#ifndef OBJ_UIOPTIONPANEL_H
#define OBJ_UIOPTIONPANEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIOptionPanel : public Object {
public:

};

//Entity Class
class EntityUIOptionPanel : public Entity {
public:

};

//Object Entity
ObjectUIOptionPanel UIOptionPanel;

//Entity Functions
void UIOptionPanel_Update();
void UIOptionPanel_EarlyUpdate();
void UIOptionPanel_LateUpdate();
void UIOptionPanel_Draw();
void UIOptionPanel_Setup(void* subtype);
void UIOptionPanel_StageLoad();
void UIOptionPanel_GetAttributes();

#endif //!OBJ_UIOPTIONPANEL_H
