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
void MSPanel_Update(void);
void MSPanel_LateUpdate(void);
void MSPanel_StaticUpdate(void);
void MSPanel_Draw(void);
void MSPanel_Create(void* data);
void MSPanel_StageLoad(void);
void MSPanel_EditorDraw(void);
void MSPanel_EditorLoad(void);
void MSPanel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MSPANEL_H
