#ifndef OBJ_PIMPOM_H
#define OBJ_PIMPOM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPimPom;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPimPom;

// Object Struct
extern ObjectPimPom *PimPom;

// Standard Entity Events
void PimPom_Update();
void PimPom_LateUpdate();
void PimPom_StaticUpdate();
void PimPom_Draw();
void PimPom_Create(void* data);
void PimPom_StageLoad();
void PimPom_EditorDraw();
void PimPom_EditorLoad();
void PimPom_Serialize();

// Extra Entity Functions


#endif //!OBJ_PIMPOM_H
