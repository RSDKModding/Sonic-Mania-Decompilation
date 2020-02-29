#ifndef OBJ_UIKEYBINDER_H
#define OBJ_UIKEYBINDER_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIKeyBinder : public Object {
public:

};

//Entity Class
class EntityUIKeyBinder : public Entity {
public:

};

//Object Entity
ObjectUIKeyBinder UIKeyBinder;

//Entity Functions
void UIKeyBinder_Update();
void UIKeyBinder_EarlyUpdate();
void UIKeyBinder_LateUpdate();
void UIKeyBinder_Draw();
void UIKeyBinder_Setup(void* subtype);
void UIKeyBinder_StageLoad();
void UIKeyBinder_GetAttributes();

#endif //!OBJ_UIKEYBINDER_H
