#ifndef OBJ_CYLINDER_H
#define OBJ_CYLINDER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[24]; //= { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13 };
    Animator value2;
    ushort value3;
} ObjectCylinder;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCylinder;

// Object Struct
extern ObjectCylinder *Cylinder;

// Standard Entity Events
void Cylinder_Update(void);
void Cylinder_LateUpdate(void);
void Cylinder_StaticUpdate(void);
void Cylinder_Draw(void);
void Cylinder_Create(void* data);
void Cylinder_StageLoad(void);
void Cylinder_EditorDraw(void);
void Cylinder_EditorLoad(void);
void Cylinder_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CYLINDER_H
