#ifndef OBJ_UIOPTIONPANEL_H
#define OBJ_UIOPTIONPANEL_H

#include "SonicMania.h"

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
void UIOptionPanel_Update(void);
void UIOptionPanel_LateUpdate(void);
void UIOptionPanel_StaticUpdate(void);
void UIOptionPanel_Draw(void);
void UIOptionPanel_Create(void* data);
void UIOptionPanel_StageLoad(void);
void UIOptionPanel_EditorDraw(void);
void UIOptionPanel_EditorLoad(void);
void UIOptionPanel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIOPTIONPANEL_H
