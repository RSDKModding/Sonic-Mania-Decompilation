#ifndef OBJ_ERZKING_H
#define OBJ_ERZKING_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectERZKing;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityERZKing;

// Object Struct
extern ObjectERZKing *ERZKing;

// Standard Entity Events
void ERZKing_Update(void);
void ERZKing_LateUpdate(void);
void ERZKing_StaticUpdate(void);
void ERZKing_Draw(void);
void ERZKing_Create(void* data);
void ERZKing_StageLoad(void);
void ERZKing_EditorDraw(void);
void ERZKing_EditorLoad(void);
void ERZKing_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ERZKING_H
