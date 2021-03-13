#ifndef OBJ_LEDPANEL_H
#define OBJ_LEDPANEL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLEDPanel;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLEDPanel;

// Object Struct
extern ObjectLEDPanel *LEDPanel;

// Standard Entity Events
void LEDPanel_Update();
void LEDPanel_LateUpdate();
void LEDPanel_StaticUpdate();
void LEDPanel_Draw();
void LEDPanel_Create(void* data);
void LEDPanel_StageLoad();
void LEDPanel_EditorDraw();
void LEDPanel_EditorLoad();
void LEDPanel_Serialize();

// Extra Entity Functions


#endif //!OBJ_LEDPANEL_H
