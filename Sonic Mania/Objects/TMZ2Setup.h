#ifndef OBJ_TMZ2SETUP_H
#define OBJ_TMZ2SETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTMZ2Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTMZ2Setup;

// Object Struct
extern ObjectTMZ2Setup *TMZ2Setup;

// Standard Entity Events
void TMZ2Setup_Update(void);
void TMZ2Setup_LateUpdate(void);
void TMZ2Setup_StaticUpdate(void);
void TMZ2Setup_Draw(void);
void TMZ2Setup_Create(void* data);
void TMZ2Setup_StageLoad(void);
void TMZ2Setup_EditorDraw(void);
void TMZ2Setup_EditorLoad(void);
void TMZ2Setup_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TMZ2SETUP_H
