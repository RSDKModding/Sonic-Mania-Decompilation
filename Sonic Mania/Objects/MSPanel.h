#ifndef OBJ_MSPANEL_H
#define OBJ_MSPANEL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMSPanel;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMSPanel;

// Object Struct
extern ObjectMSPanel *MSPanel;

// Standard Entity Events
void MSPanel_Update();
void MSPanel_LateUpdate();
void MSPanel_StaticUpdate();
void MSPanel_Draw();
void MSPanel_Create(void* data);
void MSPanel_StageLoad();
void MSPanel_EditorDraw();
void MSPanel_EditorLoad();
void MSPanel_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSPANEL_H
