#ifndef OBJ_UISHIFTER_H
#define OBJ_UISHIFTER_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIShifter : public Object {
public:

};

//Entity Class
class EntityUIShifter : public Entity {
public:

};

//Object Entity
ObjectUIShifter UIShifter;

//Entity Functions
void UIShifter_Update();
void UIShifter_EarlyUpdate();
void UIShifter_LateUpdate();
void UIShifter_Draw();
void UIShifter_Setup(void* subtype);
void UIShifter_StageLoad();
void UIShifter_GetAttributes();

#endif //!OBJ_UISHIFTER_H
