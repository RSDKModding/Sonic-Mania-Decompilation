#ifndef OBJ_CAMERA_H
#define OBJ_CAMERA_H

#include "../../SonicMania.h"

//Object Class
class ObjectCamera : public Object {
public:

};

//Entity Class
class EntityCamera : public Entity {
public:

};

//Object Entity
ObjectCamera Camera;

//Entity Functions
void Camera_Update();
void Camera_EarlyUpdate();
void Camera_LateUpdate();
void Camera_Draw();
void Camera_Setup(void* subtype);
void Camera_StageLoad();
void Camera_GetAttributes();

#endif //!OBJ_CAMERA_H
