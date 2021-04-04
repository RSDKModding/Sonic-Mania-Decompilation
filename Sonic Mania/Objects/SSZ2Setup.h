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
void SSZ2Setup_Update(void);
void SSZ2Setup_LateUpdate(void);
void SSZ2Setup_StaticUpdate(void);
void SSZ2Setup_Draw(void);
void SSZ2Setup_Create(void* data);
void SSZ2Setup_StageLoad(void);
void SSZ2Setup_EditorDraw(void);
void SSZ2Setup_EditorLoad(void);
void SSZ2Setup_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SSZ2SETUP_H
