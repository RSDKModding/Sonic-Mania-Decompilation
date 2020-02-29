#ifndef OBJ_UISAVESLOT_H
#define OBJ_UISAVESLOT_H

#include "../../SonicMania.h"

//Object Class
class ObjectUISaveSlot : public Object {
public:

};

//Entity Class
class EntityUISaveSlot : public Entity {
public:

};

//Object Entity
ObjectUISaveSlot UISaveSlot;

//Entity Functions
void UISaveSlot_Update();
void UISaveSlot_EarlyUpdate();
void UISaveSlot_LateUpdate();
void UISaveSlot_Draw();
void UISaveSlot_Setup(void* subtype);
void UISaveSlot_StageLoad();
void UISaveSlot_GetAttributes();

#endif //!OBJ_UISAVESLOT_H
