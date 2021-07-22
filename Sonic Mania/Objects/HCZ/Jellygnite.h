#ifndef OBJ_JELLYGNITE_H
#define OBJ_JELLYGNITE_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectJellygnite;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityJellygnite;

// Object Struct
extern ObjectJellygnite *Jellygnite;

// Standard Entity Events
void Jellygnite_Update(void);
void Jellygnite_LateUpdate(void);
void Jellygnite_StaticUpdate(void);
void Jellygnite_Draw(void);
void Jellygnite_Create(void* data);
void Jellygnite_StageLoad(void);
void Jellygnite_EditorDraw(void);
void Jellygnite_EditorLoad(void);
void Jellygnite_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_JELLYGNITE_H
