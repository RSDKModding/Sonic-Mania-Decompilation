#ifndef OBJ_RATTLEKILLER_H
#define OBJ_RATTLEKILLER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRattlekiller;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityRattlekiller;

// Object Struct
extern ObjectRattlekiller *Rattlekiller;

// Standard Entity Events
void Rattlekiller_Update(void);
void Rattlekiller_LateUpdate(void);
void Rattlekiller_StaticUpdate(void);
void Rattlekiller_Draw(void);
void Rattlekiller_Create(void* data);
void Rattlekiller_StageLoad(void);
void Rattlekiller_EditorDraw(void);
void Rattlekiller_EditorLoad(void);
void Rattlekiller_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_RATTLEKILLER_H
