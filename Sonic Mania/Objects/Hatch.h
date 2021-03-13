#ifndef OBJ_HATCH_H
#define OBJ_HATCH_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHatch;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHatch;

// Object Struct
extern ObjectHatch *Hatch;

// Standard Entity Events
void Hatch_Update();
void Hatch_LateUpdate();
void Hatch_StaticUpdate();
void Hatch_Draw();
void Hatch_Create(void* data);
void Hatch_StageLoad();
void Hatch_EditorDraw();
void Hatch_EditorLoad();
void Hatch_Serialize();

// Extra Entity Functions


#endif //!OBJ_HATCH_H
