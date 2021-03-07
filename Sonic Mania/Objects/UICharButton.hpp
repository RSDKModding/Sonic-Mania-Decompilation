#ifndef OBJ_UICHARBUTTON_H
#define OBJ_UICHARBUTTON_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUICharButton : Object {

};

// Entity Class
struct EntityUICharButton : Entity {

};

// Object Struct
extern ObjectUICharButton *UICharButton;

// Standard Entity Events
void UICharButton_Update();
void UICharButton_LateUpdate();
void UICharButton_StaticUpdate();
void UICharButton_Draw();
void UICharButton_Create(void* data);
void UICharButton_StageLoad();
void UICharButton_EditorDraw();
void UICharButton_EditorLoad();
void UICharButton_Serialize();

// Extra Entity Functions


#endif //!OBJ_UICHARBUTTON_H
