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
void UITAZoneModule_Update(void);
void UITAZoneModule_LateUpdate(void);
void UITAZoneModule_StaticUpdate(void);
void UITAZoneModule_Draw(void);
void UITAZoneModule_Create(void* data);
void UITAZoneModule_StageLoad(void);
void UITAZoneModule_EditorDraw(void);
void UITAZoneModule_EditorLoad(void);
void UITAZoneModule_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UITAZONEMODULE_H
