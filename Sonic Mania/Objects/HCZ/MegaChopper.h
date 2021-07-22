#ifndef OBJ_MEGACHOPPER_H
#define OBJ_MEGACHOPPER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMegaChopper;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMegaChopper;

// Object Struct
extern ObjectMegaChopper *MegaChopper;

// Standard Entity Events
void MegaChopper_Update(void);
void MegaChopper_LateUpdate(void);
void MegaChopper_StaticUpdate(void);
void MegaChopper_Draw(void);
void MegaChopper_Create(void* data);
void MegaChopper_StageLoad(void);
void MegaChopper_EditorDraw(void);
void MegaChopper_EditorLoad(void);
void MegaChopper_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MEGACHOPPER_H
