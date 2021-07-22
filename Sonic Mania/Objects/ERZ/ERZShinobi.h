#ifndef OBJ_ERZSHINOBI_H
#define OBJ_ERZSHINOBI_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectERZShinobi;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityERZShinobi;

// Object Struct
extern ObjectERZShinobi *ERZShinobi;

// Standard Entity Events
void ERZShinobi_Update(void);
void ERZShinobi_LateUpdate(void);
void ERZShinobi_StaticUpdate(void);
void ERZShinobi_Draw(void);
void ERZShinobi_Create(void* data);
void ERZShinobi_StageLoad(void);
void ERZShinobi_EditorDraw(void);
void ERZShinobi_EditorLoad(void);
void ERZShinobi_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ERZSHINOBI_H
