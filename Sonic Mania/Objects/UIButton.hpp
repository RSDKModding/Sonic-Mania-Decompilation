#ifndef OBJ_UIBUTTON_H
#define OBJ_UIBUTTON_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIButton : Object {

};

// Entity Class
struct EntityUIButton : Entity {

};

// Object Entity
extern ObjectUIButton UIButton;

// Standard Entity Events
void UIButton_Update();
void UIButton_LateUpdate();
void UIButton_StaticUpdate();
void UIButton_Draw();
void UIButton_Create(void* data);
void UIButton_StageLoad();
void UIButton_EditorDraw();
void UIButton_EditorLoad();
void UIButton_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIBUTTON_H
