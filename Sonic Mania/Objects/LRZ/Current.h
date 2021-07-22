#ifndef OBJ_CURRENT_H
#define OBJ_CURRENT_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCurrent;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCurrent;

// Object Struct
extern ObjectCurrent *Current;

// Standard Entity Events
void Current_Update(void);
void Current_LateUpdate(void);
void Current_StaticUpdate(void);
void Current_Draw(void);
void Current_Create(void* data);
void Current_StageLoad(void);
void Current_EditorDraw(void);
void Current_EditorLoad(void);
void Current_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CURRENT_H
