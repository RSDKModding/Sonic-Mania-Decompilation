#ifndef OBJ_FARPLANE_H
#define OBJ_FARPLANE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFarPlane;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFarPlane;

// Object Struct
extern ObjectFarPlane *FarPlane;

// Standard Entity Events
void FarPlane_Update();
void FarPlane_LateUpdate();
void FarPlane_StaticUpdate();
void FarPlane_Draw();
void FarPlane_Create(void* data);
void FarPlane_StageLoad();
void FarPlane_EditorDraw();
void FarPlane_EditorLoad();
void FarPlane_Serialize();

// Extra Entity Functions


#endif //!OBJ_FARPLANE_H
