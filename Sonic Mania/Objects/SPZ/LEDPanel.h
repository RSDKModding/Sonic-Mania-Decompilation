#ifndef OBJ_LEDPANEL_H
#define OBJ_LEDPANEL_H

#include "SonicMania.h"

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
void LEDPanel_Update(void);
void LEDPanel_LateUpdate(void);
void LEDPanel_StaticUpdate(void);
void LEDPanel_Draw(void);
void LEDPanel_Create(void* data);
void LEDPanel_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LEDPanel_EditorDraw(void);
void LEDPanel_EditorLoad(void);
#endif
void LEDPanel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LEDPANEL_H
