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
void PrintBlock_Update(void);
void PrintBlock_LateUpdate(void);
void PrintBlock_StaticUpdate(void);
void PrintBlock_Draw(void);
void PrintBlock_Create(void* data);
void PrintBlock_StageLoad(void);
void PrintBlock_EditorDraw(void);
void PrintBlock_EditorLoad(void);
void PrintBlock_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PRINTBLOCK_H
