#ifndef OBJ_SSZ2SETUP_H
#define OBJ_SSZ2SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSSZ2Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySSZ2Setup;

// Object Struct
extern ObjectSSZ2Setup *SSZ2Setup;

// Standard Entity Events
void SSZ2Setup_Update();
void SSZ2Setup_LateUpdate();
void SSZ2Setup_StaticUpdate();
void SSZ2Setup_Draw();
void SSZ2Setup_Create(void* data);
void SSZ2Setup_StageLoad();
void SSZ2Setup_EditorDraw();
void SSZ2Setup_EditorLoad();
void SSZ2Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_SSZ2SETUP_H
