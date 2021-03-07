#ifndef OBJ_UISAVESLOT_H
#define OBJ_UISAVESLOT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUISaveSlot : Object {

};

// Entity Class
struct EntityUISaveSlot : Entity {

};

// Object Struct
extern ObjectUISaveSlot *UISaveSlot;

// Standard Entity Events
void UISaveSlot_Update();
void UISaveSlot_LateUpdate();
void UISaveSlot_StaticUpdate();
void UISaveSlot_Draw();
void UISaveSlot_Create(void* data);
void UISaveSlot_StageLoad();
void UISaveSlot_EditorDraw();
void UISaveSlot_EditorLoad();
void UISaveSlot_Serialize();

// Extra Entity Functions


#endif //!OBJ_UISAVESLOT_H
