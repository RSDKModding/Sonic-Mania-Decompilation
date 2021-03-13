#ifndef OBJ_TMZ3SETUP_H
#define OBJ_TMZ3SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTMZ3Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTMZ3Setup;

// Object Struct
extern ObjectTMZ3Setup *TMZ3Setup;

// Standard Entity Events
void TMZ3Setup_Update();
void TMZ3Setup_LateUpdate();
void TMZ3Setup_StaticUpdate();
void TMZ3Setup_Draw();
void TMZ3Setup_Create(void* data);
void TMZ3Setup_StageLoad();
void TMZ3Setup_EditorDraw();
void TMZ3Setup_EditorLoad();
void TMZ3Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_TMZ3SETUP_H
