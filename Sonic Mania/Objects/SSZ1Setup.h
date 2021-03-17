#ifndef OBJ_SSZ1SETUP_H
#define OBJ_SSZ1SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
} ObjectSSZ1Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySSZ1Setup;

// Object Struct
extern ObjectSSZ1Setup *SSZ1Setup;

// Standard Entity Events
void SSZ1Setup_Update();
void SSZ1Setup_LateUpdate();
void SSZ1Setup_StaticUpdate();
void SSZ1Setup_Draw();
void SSZ1Setup_Create(void* data);
void SSZ1Setup_StageLoad();
void SSZ1Setup_EditorDraw();
void SSZ1Setup_EditorLoad();
void SSZ1Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_SSZ1SETUP_H
