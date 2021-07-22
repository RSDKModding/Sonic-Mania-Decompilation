#ifndef OBJ_ERZMYSTIC_H
#define OBJ_ERZMYSTIC_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectERZMystic;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityERZMystic;

// Object Struct
extern ObjectERZMystic *ERZMystic;

// Standard Entity Events
void ERZMystic_Update(void);
void ERZMystic_LateUpdate(void);
void ERZMystic_StaticUpdate(void);
void ERZMystic_Draw(void);
void ERZMystic_Create(void* data);
void ERZMystic_StageLoad(void);
void ERZMystic_EditorDraw(void);
void ERZMystic_EditorLoad(void);
void ERZMystic_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ERZMYSTIC_H
