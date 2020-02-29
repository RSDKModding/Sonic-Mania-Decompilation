#ifndef OBJ_UIWIDGETS_H
#define OBJ_UIWIDGETS_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIWidgets : public Object {
public:

};

//Entity Class
class EntityUIWidgets : public Entity {
public:

};

//Object Entity
ObjectUIWidgets UIWidgets;

//Entity Functions
void UIWidgets_Update();
void UIWidgets_EarlyUpdate();
void UIWidgets_LateUpdate();
void UIWidgets_Draw();
void UIWidgets_Setup(void* subtype);
void UIWidgets_StageLoad();
void UIWidgets_GetAttributes();

#endif //!OBJ_UIWIDGETS_H
