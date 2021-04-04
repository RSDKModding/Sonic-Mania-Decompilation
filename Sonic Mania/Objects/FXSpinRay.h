#ifndef OBJ_FXSPINRAY_H
#define OBJ_FXSPINRAY_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFXSpinRay;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFXSpinRay;

// Object Struct
extern ObjectFXSpinRay *FXSpinRay;

// Standard Entity Events
void FXSpinRay_Update(void);
void FXSpinRay_LateUpdate(void);
void FXSpinRay_StaticUpdate(void);
void FXSpinRay_Draw(void);
void FXSpinRay_Create(void* data);
void FXSpinRay_StageLoad(void);
void FXSpinRay_EditorDraw(void);
void FXSpinRay_EditorLoad(void);
void FXSpinRay_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FXSPINRAY_H
