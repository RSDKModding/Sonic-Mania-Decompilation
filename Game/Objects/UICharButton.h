#ifndef OBJ_UICHARBUTTON_H
#define OBJ_UICHARBUTTON_H

#include "../../SonicMania.h"

//Object Class
class ObjectUICharButton : public Object {
public:

};

//Entity Class
class EntityUICharButton : public Entity {
public:

};

//Object Entity
ObjectUICharButton UICharButton;

//Entity Functions
void UICharButton_Update();
void UICharButton_EarlyUpdate();
void UICharButton_LateUpdate();
void UICharButton_Draw();
void UICharButton_Setup(void* subtype);
void UICharButton_StageLoad();
void UICharButton_GetAttributes();

#endif //!OBJ_UICHARBUTTON_H
