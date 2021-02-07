#ifndef OBJ_UIINFOLABEL_H
#define OBJ_UIINFOLABEL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIInfoLabel : Object{

};

// Entity Class
struct EntityUIInfoLabel : Entity {

};

// Object Struct
extern ObjectUIInfoLabel *UIInfoLabel;

// Standard Entity Events
void UIInfoLabel_Update();
void UIInfoLabel_LateUpdate();
void UIInfoLabel_StaticUpdate();
void UIInfoLabel_Draw();
void UIInfoLabel_Create(void* data);
void UIInfoLabel_StageLoad();
void UIInfoLabel_EditorDraw();
void UIInfoLabel_EditorLoad();
void UIInfoLabel_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIINFOLABEL_H
