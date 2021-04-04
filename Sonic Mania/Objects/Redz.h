#ifndef OBJ_REDZ_H
#define OBJ_REDZ_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRedz;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityRedz;

// Object Entity
extern ObjectRedz *Redz;

// Standard Entity Events
void Redz_Update(void);
void Redz_LateUpdate(void);
void Redz_StaticUpdate(void);
void Redz_Draw(void);
void Redz_Create(void* data);
void Redz_StageLoad(void);
void Redz_EditorDraw(void);
void Redz_EditorLoad(void);
void Redz_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_REDZ_H
