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
void TMZ3Setup_Update(void);
void TMZ3Setup_LateUpdate(void);
void TMZ3Setup_StaticUpdate(void);
void TMZ3Setup_Draw(void);
void TMZ3Setup_Create(void* data);
void TMZ3Setup_StageLoad(void);
void TMZ3Setup_EditorDraw(void);
void TMZ3Setup_EditorLoad(void);
void TMZ3Setup_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TMZ3SETUP_H
