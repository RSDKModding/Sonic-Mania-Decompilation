#ifndef OBJ_BLASTOID_H
#define OBJ_BLASTOID_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBlastoid;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBlastoid;

// Object Struct
extern ObjectBlastoid *Blastoid;

// Standard Entity Events
void Blastoid_Update(void);
void Blastoid_LateUpdate(void);
void Blastoid_StaticUpdate(void);
void Blastoid_Draw(void);
void Blastoid_Create(void* data);
void Blastoid_StageLoad(void);
void Blastoid_EditorDraw(void);
void Blastoid_EditorLoad(void);
void Blastoid_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BLASTOID_H
