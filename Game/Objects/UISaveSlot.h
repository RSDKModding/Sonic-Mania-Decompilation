#ifndef OBJ_UISAVESLOT_H
#define OBJ_UISAVESLOT_H

//Object Class
class UISaveSlot : Object {

};

//Entity Class
class EntityUISaveSlot : Entity {

};

//Entity Functions
void UISaveSlot_Update();
void UISaveSlot_EarlyUpdate();
void UISaveSlot_LateUpdate();
void UISaveSlot_Draw();
void UISaveSlot_Setup(void* subtype);
void UISaveSlot_StageLoad();
void UISaveSlot_GetAttributes();

#endif //!OBJ_UISAVESLOT_H
