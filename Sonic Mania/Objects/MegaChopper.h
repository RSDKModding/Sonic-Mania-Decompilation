#ifndef OBJ_MEGACHOPPER_H
#define OBJ_MEGACHOPPER_H

#include "../SonicMania.h"

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
void MegaChopper_Update();
void MegaChopper_LateUpdate();
void MegaChopper_StaticUpdate();
void MegaChopper_Draw();
void MegaChopper_Create(void* data);
void MegaChopper_StageLoad();
void MegaChopper_EditorDraw();
void MegaChopper_EditorLoad();
void MegaChopper_Serialize();

// Extra Entity Functions


#endif //!OBJ_MEGACHOPPER_H
