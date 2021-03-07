#ifndef OBJ_UIWIDGETS_H
#define OBJ_UIWIDGETS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIWidgets : Object {

};

// Entity Class
struct EntityUIWidgets : Entity {

};

// Object Struct
extern ObjectUIWidgets *UIWidgets;

// Standard Entity Events
void UIWidgets_Update();
void UIWidgets_LateUpdate();
void UIWidgets_StaticUpdate();
void UIWidgets_Draw();
void UIWidgets_Create(void* data);
void UIWidgets_StageLoad();
void UIWidgets_EditorDraw();
void UIWidgets_EditorLoad();
void UIWidgets_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIWIDGETS_H
