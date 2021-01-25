#ifndef OBJ_UIMEDALLIONPANEL_H
#define OBJ_UIMEDALLIONPANEL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIMedallionPanel : Object {

};

// Entity Class
struct EntityUIMedallionPanel : Entity {

};

// Object Entity
extern ObjectUIMedallionPanel UIMedallionPanel;

// Standard Entity Events
void UIMedallionPanel_Update();
void UIMedallionPanel_LateUpdate();
void UIMedallionPanel_StaticUpdate();
void UIMedallionPanel_Draw();
void UIMedallionPanel_Create(void* data);
void UIMedallionPanel_StageLoad();
void UIMedallionPanel_EditorDraw();
void UIMedallionPanel_EditorLoad();
void UIMedallionPanel_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIMEDALLIONPANEL_H
