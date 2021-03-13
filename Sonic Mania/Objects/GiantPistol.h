#ifndef OBJ_GIANTPISTOL_H
#define OBJ_GIANTPISTOL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectGiantPistol;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityGiantPistol;

// Object Struct
extern ObjectGiantPistol *GiantPistol;

// Standard Entity Events
void GiantPistol_Update();
void GiantPistol_LateUpdate();
void GiantPistol_StaticUpdate();
void GiantPistol_Draw();
void GiantPistol_Create(void* data);
void GiantPistol_StageLoad();
void GiantPistol_EditorDraw();
void GiantPistol_EditorLoad();
void GiantPistol_Serialize();

// Extra Entity Functions


#endif //!OBJ_GIANTPISTOL_H
