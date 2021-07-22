#ifndef OBJ_GASPLATFORM_H
#define OBJ_GASPLATFORM_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectGasPlatform;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityGasPlatform;

// Object Struct
extern ObjectGasPlatform *GasPlatform;

// Standard Entity Events
void GasPlatform_Update(void);
void GasPlatform_LateUpdate(void);
void GasPlatform_StaticUpdate(void);
void GasPlatform_Draw(void);
void GasPlatform_Create(void* data);
void GasPlatform_StageLoad(void);
void GasPlatform_EditorDraw(void);
void GasPlatform_EditorLoad(void);
void GasPlatform_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_GASPLATFORM_H
