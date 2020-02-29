#ifndef OBJ_UITRANSITION_H
#define OBJ_UITRANSITION_H

#include "../../SonicMania.h"

//Object Class
class ObjectUITransition : public Object {
public:

};

//Entity Class
class EntityUITransition : public Entity {
public:

};

//Object Entity
ObjectUITransition UITransition;

//Entity Functions
void UITransition_Update();
void UITransition_EarlyUpdate();
void UITransition_LateUpdate();
void UITransition_Draw();
void UITransition_Setup(void* subtype);
void UITransition_StageLoad();
void UITransition_GetAttributes();

#endif //!OBJ_UITRANSITION_H
