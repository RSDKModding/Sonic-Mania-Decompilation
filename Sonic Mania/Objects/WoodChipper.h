#ifndef OBJ_WOODCHIPPER_H
#define OBJ_WOODCHIPPER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectWoodChipper;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWoodChipper;

// Object Struct
extern ObjectWoodChipper *WoodChipper;

// Standard Entity Events
void WoodChipper_Update(void);
void WoodChipper_LateUpdate(void);
void WoodChipper_StaticUpdate(void);
void WoodChipper_Draw(void);
void WoodChipper_Create(void* data);
void WoodChipper_StageLoad(void);
void WoodChipper_EditorDraw(void);
void WoodChipper_EditorLoad(void);
void WoodChipper_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_WOODCHIPPER_H
