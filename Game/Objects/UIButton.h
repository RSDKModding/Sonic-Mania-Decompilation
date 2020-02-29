#ifndef OBJ_UIBUTTON_H
#define OBJ_UIBUTTON_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIButton : public Object {
public:

};

//Entity Class
class EntityUIButton : public Entity {
public:

};

//Object Entity
ObjectUIButton UIButton;

//Entity Functions
void UIButton_Update();
void UIButton_EarlyUpdate();
void UIButton_LateUpdate();
void UIButton_Draw();
void UIButton_Setup(void* subtype);
void UIButton_StageLoad();
void UIButton_GetAttributes();

#endif //!OBJ_UIBUTTON_H
