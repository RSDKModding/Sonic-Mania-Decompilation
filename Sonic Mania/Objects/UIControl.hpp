#ifndef OBJ_UICONTROL_H
#define OBJ_UICONTROL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIControl : Object {

};

// Entity Class
struct EntityUIControl : Entity {

};

// Object Struct
extern ObjectUIControl *UIControl;

// Standard Entity Events
void UIControl_Update();
void UIControl_LateUpdate();
void UIControl_StaticUpdate();
void UIControl_Draw();
void UIControl_Create(void* data);
void UIControl_StageLoad();
void UIControl_EditorDraw();
void UIControl_EditorLoad();
void UIControl_Serialize();

// Extra Entity Functions


#endif //!OBJ_UICONTROL_H
