#ifndef OBJ_UFO_CAMERA_H
#define OBJ_UFO_CAMERA_H

#include "../../SonicMania.h"

//Object Class
class ObjectUFO_Camera : public Object {
public:

};

//Entity Class
class EntityUFO_Camera : public Entity {
public:

};

//Object Entity
ObjectUFO_Camera UFO_Camera;

//Entity Functions
void UFO_Camera_Update();
void UFO_Camera_EarlyUpdate();
void UFO_Camera_LateUpdate();
void UFO_Camera_Draw();
void UFO_Camera_Setup(void* subtype);
void UFO_Camera_StageLoad();
void UFO_Camera_GetAttributes();

#endif //!OBJ_UFO_CAMERA_H
