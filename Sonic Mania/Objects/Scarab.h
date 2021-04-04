#ifndef OBJ_SCARAB_H
#define OBJ_SCARAB_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectScarab;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityScarab;

// Object Struct
extern ObjectScarab *Scarab;

// Standard Entity Events
void Scarab_Update(void);
void Scarab_LateUpdate(void);
void Scarab_StaticUpdate(void);
void Scarab_Draw(void);
void Scarab_Create(void* data);
void Scarab_StageLoad(void);
void Scarab_EditorDraw(void);
void Scarab_EditorLoad(void);
void Scarab_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SCARAB_H
