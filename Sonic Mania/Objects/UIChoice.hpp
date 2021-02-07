#ifndef OBJ_UICHOICE_H
#define OBJ_UICHOICE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIChoice : Object{

};

// Entity Class
struct EntityUIChoice : Entity {

};

// Object Struct
extern ObjectUIChoice *UIChoice;

// Standard Entity Events
void UIChoice_Update();
void UIChoice_LateUpdate();
void UIChoice_StaticUpdate();
void UIChoice_Draw();
void UIChoice_Create(void* data);
void UIChoice_StageLoad();
void UIChoice_EditorDraw();
void UIChoice_EditorLoad();
void UIChoice_Serialize();

// Extra Entity Functions


#endif //!OBJ_UICHOICE_H
