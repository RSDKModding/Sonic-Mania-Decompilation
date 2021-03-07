#ifndef OBJ_UIOPTIONPANEL_H
#define OBJ_UIOPTIONPANEL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIOptionPanel : Object {

};

// Entity Class
struct EntityUIOptionPanel : Entity {

};

// Object Struct
extern ObjectUIOptionPanel *UIOptionPanel;

// Standard Entity Events
void UIOptionPanel_Update();
void UIOptionPanel_LateUpdate();
void UIOptionPanel_StaticUpdate();
void UIOptionPanel_Draw();
void UIOptionPanel_Create(void* data);
void UIOptionPanel_StageLoad();
void UIOptionPanel_EditorDraw();
void UIOptionPanel_EditorLoad();
void UIOptionPanel_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIOPTIONPANEL_H
