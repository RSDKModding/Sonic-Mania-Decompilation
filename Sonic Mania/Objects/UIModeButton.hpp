#ifndef OBJ_UIMODEBUTTON_H
#define OBJ_UIMODEBUTTON_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIModeButton : Object {

};

// Entity Class
struct EntityUIModeButton : Entity {

};

// Object Struct
extern ObjectUIModeButton *UIModeButton;

// Standard Entity Events
void UIModeButton_Update();
void UIModeButton_LateUpdate();
void UIModeButton_StaticUpdate();
void UIModeButton_Draw();
void UIModeButton_Create(void* data);
void UIModeButton_StageLoad();
void UIModeButton_EditorDraw();
void UIModeButton_EditorLoad();
void UIModeButton_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIMODEBUTTON_H
