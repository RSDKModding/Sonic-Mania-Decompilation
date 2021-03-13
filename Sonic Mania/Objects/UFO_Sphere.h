#ifndef OBJ_UFO_SPHERE_H
#define OBJ_UFO_SPHERE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1;
    int value2[4]; //= { 12, 12, 16, 16 };
    ushort value3;
    ushort value4;
    ushort value5;
} ObjectUFO_Sphere;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUFO_Sphere;

// Object Struct
extern ObjectUFO_Sphere *UFO_Sphere;

// Standard Entity Events
void UFO_Sphere_Update();
void UFO_Sphere_LateUpdate();
void UFO_Sphere_StaticUpdate();
void UFO_Sphere_Draw();
void UFO_Sphere_Create(void* data);
void UFO_Sphere_StageLoad();
void UFO_Sphere_EditorDraw();
void UFO_Sphere_EditorLoad();
void UFO_Sphere_Serialize();

// Extra Entity Functions


#endif //!OBJ_UFO_SPHERE_H
