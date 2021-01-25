#ifndef OBJ_UIPOPOVER_H
#define OBJ_UIPOPOVER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIPopover : Object {

};

// Entity Class
struct EntityUIPopover : Entity {

};

// Object Entity
extern ObjectUIPopover UIPopover;

// Standard Entity Events
void UIPopover_Update();
void UIPopover_LateUpdate();
void UIPopover_StaticUpdate();
void UIPopover_Draw();
void UIPopover_Create(void* data);
void UIPopover_StageLoad();
void UIPopover_EditorDraw();
void UIPopover_EditorLoad();
void UIPopover_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIPOPOVER_H
