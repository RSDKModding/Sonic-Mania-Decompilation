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
void DAControl_Update(void);
void DAControl_LateUpdate(void);
void DAControl_StaticUpdate(void);
void DAControl_Draw(void);
void DAControl_Create(void* data);
void DAControl_StageLoad(void);
void DAControl_EditorDraw(void);
void DAControl_EditorLoad(void);
void DAControl_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DACONTROL_H
