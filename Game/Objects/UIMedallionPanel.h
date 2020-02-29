#ifndef OBJ_UIMEDALLIONPANEL_H
#define OBJ_UIMEDALLIONPANEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIMedallionPanel : public Object {
public:

};

//Entity Class
class EntityUIMedallionPanel : public Entity {
public:

};

//Object Entity
ObjectUIMedallionPanel UIMedallionPanel;

//Entity Functions
void UIMedallionPanel_Update();
void UIMedallionPanel_EarlyUpdate();
void UIMedallionPanel_LateUpdate();
void UIMedallionPanel_Draw();
void UIMedallionPanel_Setup(void* subtype);
void UIMedallionPanel_StageLoad();
void UIMedallionPanel_GetAttributes();

#endif //!OBJ_UIMEDALLIONPANEL_H
