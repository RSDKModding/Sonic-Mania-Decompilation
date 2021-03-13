#ifndef OBJ_UIOPTIONPANEL_H
#define OBJ_UIOPTIONPANEL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIOptionPanel;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUIOptionPanel;

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
