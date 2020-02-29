#ifndef OBJ_UIPOPOVER_H
#define OBJ_UIPOPOVER_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIPopover : public Object {
public:

};

//Entity Class
class EntityUIPopover : public Entity {
public:

};

//Object Entity
ObjectUIPopover UIPopover;

//Entity Functions
void UIPopover_Update();
void UIPopover_EarlyUpdate();
void UIPopover_LateUpdate();
void UIPopover_Draw();
void UIPopover_Setup(void* subtype);
void UIPopover_StageLoad();
void UIPopover_GetAttributes();

#endif //!OBJ_UIPOPOVER_H
