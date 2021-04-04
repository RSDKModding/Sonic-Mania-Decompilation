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
void GiantPistol_Update(void);
void GiantPistol_LateUpdate(void);
void GiantPistol_StaticUpdate(void);
void GiantPistol_Draw(void);
void GiantPistol_Create(void* data);
void GiantPistol_StageLoad(void);
void GiantPistol_EditorDraw(void);
void GiantPistol_EditorLoad(void);
void GiantPistol_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_GIANTPISTOL_H
