#ifndef OBJ_UIBUTTONLABEL_H
#define OBJ_UIBUTTONLABEL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIButtonLabel : Object{

};

// Entity Class
struct EntityUIButtonLabel : Entity {

};

// Object Struct
extern ObjectUIButtonLabel *UIButtonLabel;

// Standard Entity Events
void UIButtonLabel_Update();
void UIButtonLabel_LateUpdate();
void UIButtonLabel_StaticUpdate();
void UIButtonLabel_Draw();
void UIButtonLabel_Create(void* data);
void UIButtonLabel_StageLoad();
void UIButtonLabel_EditorDraw();
void UIButtonLabel_EditorLoad();
void UIButtonLabel_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIBUTTONLABEL_H
