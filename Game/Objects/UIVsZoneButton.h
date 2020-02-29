#ifndef OBJ_UIVSZONEBUTTON_H
#define OBJ_UIVSZONEBUTTON_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIVsZoneButton : public Object {
public:

};

//Entity Class
class EntityUIVsZoneButton : public Entity {
public:

};

//Object Entity
ObjectUIVsZoneButton UIVsZoneButton;

//Entity Functions
void UIVsZoneButton_Update();
void UIVsZoneButton_EarlyUpdate();
void UIVsZoneButton_LateUpdate();
void UIVsZoneButton_Draw();
void UIVsZoneButton_Setup(void* subtype);
void UIVsZoneButton_StageLoad();
void UIVsZoneButton_GetAttributes();

#endif //!OBJ_UIVSZONEBUTTON_H
