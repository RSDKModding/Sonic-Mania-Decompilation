#ifndef OBJ_ACETONE_H
#define OBJ_ACETONE_H

#include "../SonicMania.h"

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
void Acetone_Update();
void Acetone_LateUpdate();
void Acetone_StaticUpdate();
void Acetone_Draw();
void Acetone_Create(void* data);
void Acetone_StageLoad();
void Acetone_EditorDraw();
void Acetone_EditorLoad();
void Acetone_Serialize();

// Extra Entity Functions


#endif //!OBJ_ACETONE_H
