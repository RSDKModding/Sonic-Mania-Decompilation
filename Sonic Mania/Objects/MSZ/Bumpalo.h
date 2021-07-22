#ifndef OBJ_BUMPALO_H
#define OBJ_BUMPALO_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBumpalo;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBumpalo;

// Object Struct
extern ObjectBumpalo *Bumpalo;

// Standard Entity Events
void Bumpalo_Update(void);
void Bumpalo_LateUpdate(void);
void Bumpalo_StaticUpdate(void);
void Bumpalo_Draw(void);
void Bumpalo_Create(void* data);
void Bumpalo_StageLoad(void);
void Bumpalo_EditorDraw(void);
void Bumpalo_EditorLoad(void);
void Bumpalo_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BUMPALO_H
