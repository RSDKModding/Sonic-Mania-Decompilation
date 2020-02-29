#ifndef OBJ_CAMERA_H
#define OBJ_CAMERA_H

//Object Class
class Camera : Object {

};

//Entity Class
class EntityCamera : Entity {

};

//Entity Functions
void Camera_Update();
void Camera_EarlyUpdate();
void Camera_LateUpdate();
void Camera_Draw();
void Camera_Setup(void* subtype);
void Camera_StageLoad();
void Camera_GetAttributes();

#endif //!OBJ_CAMERA_H
