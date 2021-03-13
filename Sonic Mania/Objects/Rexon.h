#ifndef OBJ_REXON_H
#define OBJ_REXON_H

#include "../SonicMania.h"

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
void Rexon_Update();
void Rexon_LateUpdate();
void Rexon_StaticUpdate();
void Rexon_Draw();
void Rexon_Create(void* data);
void Rexon_StageLoad();
void Rexon_EditorDraw();
void Rexon_EditorLoad();
void Rexon_Serialize();

// Extra Entity Functions


#endif //!OBJ_REXON_H
