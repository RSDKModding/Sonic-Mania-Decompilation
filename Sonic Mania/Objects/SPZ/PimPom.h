#ifndef OBJ_PIMPOM_H
#define OBJ_PIMPOM_H

#include "SonicMania.h"

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
void PimPom_Update(void);
void PimPom_LateUpdate(void);
void PimPom_StaticUpdate(void);
void PimPom_Draw(void);
void PimPom_Create(void* data);
void PimPom_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PimPom_EditorDraw(void);
void PimPom_EditorLoad(void);
#endif
void PimPom_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PIMPOM_H
