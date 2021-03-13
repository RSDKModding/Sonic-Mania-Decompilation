#ifndef OBJ_GONDOLA_H
#define OBJ_GONDOLA_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectGondola;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityGondola;

// Object Struct
extern ObjectGondola *Gondola;

// Standard Entity Events
void Gondola_Update();
void Gondola_LateUpdate();
void Gondola_StaticUpdate();
void Gondola_Draw();
void Gondola_Create(void* data);
void Gondola_StageLoad();
void Gondola_EditorDraw();
void Gondola_EditorLoad();
void Gondola_Serialize();

// Extra Entity Functions


#endif //!OBJ_GONDOLA_H
