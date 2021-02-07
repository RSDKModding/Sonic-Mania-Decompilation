#ifndef OBJ_UIKEYBINDER_H
#define OBJ_UIKEYBINDER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIKeyBinder : Object{

};

// Entity Class
struct EntityUIKeyBinder : Entity {

};

// Object Struct
extern ObjectUIKeyBinder *UIKeyBinder;

// Standard Entity Events
void UIKeyBinder_Update();
void UIKeyBinder_LateUpdate();
void UIKeyBinder_StaticUpdate();
void UIKeyBinder_Draw();
void UIKeyBinder_Create(void* data);
void UIKeyBinder_StageLoad();
void UIKeyBinder_EditorDraw();
void UIKeyBinder_EditorLoad();
void UIKeyBinder_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIKEYBINDER_H
