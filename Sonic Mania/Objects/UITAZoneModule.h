#ifndef OBJ_UITAZONEMODULE_H
#define OBJ_UITAZONEMODULE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUITAZoneModule;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUITAZoneModule;

// Object Struct
extern ObjectUITAZoneModule *UITAZoneModule;

// Standard Entity Events
void UITAZoneModule_Update();
void UITAZoneModule_LateUpdate();
void UITAZoneModule_StaticUpdate();
void UITAZoneModule_Draw();
void UITAZoneModule_Create(void* data);
void UITAZoneModule_StageLoad();
void UITAZoneModule_EditorDraw();
void UITAZoneModule_EditorLoad();
void UITAZoneModule_Serialize();

// Extra Entity Functions


#endif //!OBJ_UITAZONEMODULE_H
