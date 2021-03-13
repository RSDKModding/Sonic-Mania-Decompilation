#ifndef OBJ_DACONTROL_H
#define OBJ_DACONTROL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDAControl;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDAControl;

// Object Struct
extern ObjectDAControl *DAControl;

// Standard Entity Events
void DAControl_Update();
void DAControl_LateUpdate();
void DAControl_StaticUpdate();
void DAControl_Draw();
void DAControl_Create(void* data);
void DAControl_StageLoad();
void DAControl_EditorDraw();
void DAControl_EditorLoad();
void DAControl_Serialize();

// Extra Entity Functions


#endif //!OBJ_DACONTROL_H
