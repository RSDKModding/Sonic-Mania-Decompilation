#ifndef OBJ_BUTTON_H
#define OBJ_BUTTON_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectButton : Object {

};

// Entity Class
struct EntityButton : Entity {

};

// Object Entity
extern ObjectButton Button;

// Standard Entity Events
void Button_Update();
void Button_LateUpdate();
void Button_StaticUpdate();
void Button_Draw();
void Button_Create(void* data);
void Button_StageLoad();
void Button_EditorDraw();
void Button_EditorLoad();
void Button_Serialize();

// Extra Entity Functions


#endif //!OBJ_BUTTON_H
