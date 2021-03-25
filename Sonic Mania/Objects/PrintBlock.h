#ifndef OBJ_PRINTBLOCK_H
#define OBJ_PRINTBLOCK_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPrintBlock;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPrintBlock;

// Object Struct
extern ObjectPrintBlock *PrintBlock;

// Standard Entity Events
void PrintBlock_Update();
void PrintBlock_LateUpdate();
void PrintBlock_StaticUpdate();
void PrintBlock_Draw();
void PrintBlock_Create(void* data);
void PrintBlock_StageLoad();
void PrintBlock_EditorDraw();
void PrintBlock_EditorLoad();
void PrintBlock_Serialize();

// Extra Entity Functions


#endif //!OBJ_PRINTBLOCK_H