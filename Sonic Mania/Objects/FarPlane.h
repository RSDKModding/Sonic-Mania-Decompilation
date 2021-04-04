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
void FarPlane_Update(void);
void FarPlane_LateUpdate(void);
void FarPlane_StaticUpdate(void);
void FarPlane_Draw(void);
void FarPlane_Create(void* data);
void FarPlane_StageLoad(void);
void FarPlane_EditorDraw(void);
void FarPlane_EditorLoad(void);
void FarPlane_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FARPLANE_H
