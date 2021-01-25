#ifndef OBJ_UITEXT_H
#define OBJ_UITEXT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIText : Object {

};

// Entity Class
struct EntityUIText : Entity {

};

// Object Entity
extern ObjectUIText UIText;

// Standard Entity Events
void UIText_Update();
void UIText_LateUpdate();
void UIText_StaticUpdate();
void UIText_Draw();
void UIText_Create(void* data);
void UIText_StageLoad();
void UIText_EditorDraw();
void UIText_EditorLoad();
void UIText_Serialize();

// Extra Entity Functions


#endif //!OBJ_UITEXT_H
