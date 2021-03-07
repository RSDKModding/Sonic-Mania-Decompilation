#ifndef OBJ_UITRANSITION_H
#define OBJ_UITRANSITION_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUITransition : Object {

};

// Entity Class
struct EntityUITransition : Entity {

};

// Object Struct
extern ObjectUITransition *UITransition;

// Standard Entity Events
void UITransition_Update();
void UITransition_LateUpdate();
void UITransition_StaticUpdate();
void UITransition_Draw();
void UITransition_Create(void* data);
void UITransition_StageLoad();
void UITransition_EditorDraw();
void UITransition_EditorLoad();
void UITransition_Serialize();

// Extra Entity Functions


#endif //!OBJ_UITRANSITION_H
