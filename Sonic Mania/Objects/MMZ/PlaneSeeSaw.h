#ifndef OBJ_PLANESEESAW_H
#define OBJ_PLANESEESAW_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPlaneSeeSaw;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPlaneSeeSaw;

// Object Struct
extern ObjectPlaneSeeSaw *PlaneSeeSaw;

// Standard Entity Events
void PlaneSeeSaw_Update(void);
void PlaneSeeSaw_LateUpdate(void);
void PlaneSeeSaw_StaticUpdate(void);
void PlaneSeeSaw_Draw(void);
void PlaneSeeSaw_Create(void* data);
void PlaneSeeSaw_StageLoad(void);
void PlaneSeeSaw_EditorDraw(void);
void PlaneSeeSaw_EditorLoad(void);
void PlaneSeeSaw_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PLANESEESAW_H
