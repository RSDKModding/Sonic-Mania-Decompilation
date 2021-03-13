#ifndef OBJ_TUBINAUT_H
#define OBJ_TUBINAUT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTubinaut;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTubinaut;

// Object Struct
extern ObjectTubinaut *Tubinaut;

// Standard Entity Events
void Tubinaut_Update();
void Tubinaut_LateUpdate();
void Tubinaut_StaticUpdate();
void Tubinaut_Draw();
void Tubinaut_Create(void* data);
void Tubinaut_StageLoad();
void Tubinaut_EditorDraw();
void Tubinaut_EditorLoad();
void Tubinaut_Serialize();

// Extra Entity Functions


#endif //!OBJ_TUBINAUT_H
