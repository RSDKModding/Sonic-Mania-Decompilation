#ifndef OBJ_DANGO_H
#define OBJ_DANGO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDango;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDango;

// Object Struct
extern ObjectDango *Dango;

// Standard Entity Events
void Dango_Update();
void Dango_LateUpdate();
void Dango_StaticUpdate();
void Dango_Draw();
void Dango_Create(void* data);
void Dango_StageLoad();
void Dango_EditorDraw();
void Dango_EditorLoad();
void Dango_Serialize();

// Extra Entity Functions


#endif //!OBJ_DANGO_H
