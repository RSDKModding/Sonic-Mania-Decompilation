#ifndef OBJ_TMZ1SETUP_H
#define OBJ_TMZ1SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTMZ1Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTMZ1Setup;

// Object Struct
extern ObjectTMZ1Setup *TMZ1Setup;

// Standard Entity Events
void TMZ1Setup_Update();
void TMZ1Setup_LateUpdate();
void TMZ1Setup_StaticUpdate();
void TMZ1Setup_Draw();
void TMZ1Setup_Create(void* data);
void TMZ1Setup_StageLoad();
void TMZ1Setup_EditorDraw();
void TMZ1Setup_EditorLoad();
void TMZ1Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_TMZ1SETUP_H
