#ifndef OBJ_UIRESPICKER_H
#define OBJ_UIRESPICKER_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIResPicker : public Object {
public:

};

//Entity Class
class EntityUIResPicker : public Entity {
public:

};

//Object Entity
ObjectUIResPicker UIResPicker;

//Entity Functions
void UIResPicker_Update();
void UIResPicker_EarlyUpdate();
void UIResPicker_LateUpdate();
void UIResPicker_Draw();
void UIResPicker_Setup(void* subtype);
void UIResPicker_StageLoad();
void UIResPicker_GetAttributes();

#endif //!OBJ_UIRESPICKER_H
