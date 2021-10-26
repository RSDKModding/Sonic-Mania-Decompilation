#ifndef OBJ_REXON_H
#define OBJ_REXON_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRexon;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityRexon;

// Object Struct
extern ObjectRexon *Rexon;

// Standard Entity Events
void Rexon_Update(void);
void Rexon_LateUpdate(void);
void Rexon_StaticUpdate(void);
void Rexon_Draw(void);
void Rexon_Create(void* data);
void Rexon_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Rexon_EditorDraw(void);
void Rexon_EditorLoad(void);
#endif
void Rexon_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_REXON_H
