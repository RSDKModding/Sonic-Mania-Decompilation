#ifndef OBJ_ACETONE_H
#define OBJ_ACETONE_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectAcetone;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityAcetone;

// Object Entity
extern ObjectAcetone *Acetone;

// Standard Entity Events
void Acetone_Update(void);
void Acetone_LateUpdate(void);
void Acetone_StaticUpdate(void);
void Acetone_Draw(void);
void Acetone_Create(void* data);
void Acetone_StageLoad(void);
void Acetone_EditorDraw(void);
void Acetone_EditorLoad(void);
void Acetone_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ACETONE_H
