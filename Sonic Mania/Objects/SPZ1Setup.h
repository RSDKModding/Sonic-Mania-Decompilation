#ifndef OBJ_SPZ1SETUP_H
#define OBJ_SPZ1SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSPZ1Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySPZ1Setup;

// Object Struct
extern ObjectSPZ1Setup *SPZ1Setup;

// Standard Entity Events
void SPZ1Setup_Update();
void SPZ1Setup_LateUpdate();
void SPZ1Setup_StaticUpdate();
void SPZ1Setup_Draw();
void SPZ1Setup_Create(void* data);
void SPZ1Setup_StageLoad();
void SPZ1Setup_EditorDraw();
void SPZ1Setup_EditorLoad();
void SPZ1Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPZ1SETUP_H
