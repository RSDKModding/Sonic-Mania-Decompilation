#ifndef OBJ_BUGGERNAUT_H
#define OBJ_BUGGERNAUT_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBuggernaut;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBuggernaut;

// Object Struct
extern ObjectBuggernaut *Buggernaut;

// Standard Entity Events
void Buggernaut_Update(void);
void Buggernaut_LateUpdate(void);
void Buggernaut_StaticUpdate(void);
void Buggernaut_Draw(void);
void Buggernaut_Create(void* data);
void Buggernaut_StageLoad(void);
void Buggernaut_EditorDraw(void);
void Buggernaut_EditorLoad(void);
void Buggernaut_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BUGGERNAUT_H
