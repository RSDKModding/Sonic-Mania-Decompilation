#ifndef OBJ_UISLIDER_H
#define OBJ_UISLIDER_H

#include "../../SonicMania.h"

//Object Class
class ObjectUISlider : public Object {
public:

};

//Entity Class
class EntityUISlider : public Entity {
public:

};

//Object Entity
ObjectUISlider UISlider;

//Entity Functions
void UISlider_Update();
void UISlider_EarlyUpdate();
void UISlider_LateUpdate();
void UISlider_Draw();
void UISlider_Setup(void* subtype);
void UISlider_StageLoad();
void UISlider_GetAttributes();

#endif //!OBJ_UISLIDER_H
