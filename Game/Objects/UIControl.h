#ifndef OBJ_UICONTROL_H
#define OBJ_UICONTROL_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIControl : public Object {
public:

};

//Entity Class
class EntityUIControl : public Entity {
public:

};

//Object Entity
ObjectUIControl UIControl;

//Entity Functions
void UIControl_Update();
void UIControl_EarlyUpdate();
void UIControl_LateUpdate();
void UIControl_Draw();
void UIControl_Setup(void* subtype);
void UIControl_StageLoad();
void UIControl_GetAttributes();

#endif //!OBJ_UICONTROL_H
