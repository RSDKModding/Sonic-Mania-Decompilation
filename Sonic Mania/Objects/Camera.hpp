#ifndef OBJ_CAMERA_H
#define OBJ_CAMERA_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCamera : Object{

};

// Entity Class
struct EntityCamera : Entity {

};

// Object Struct
extern ObjectCamera *Camera;

// Standard Entity Events
void Camera_Update();
void Camera_LateUpdate();
void Camera_StaticUpdate();
void Camera_Draw();
void Camera_Create(void* data);
void Camera_StageLoad();
void Camera_EditorDraw();
void Camera_EditorLoad();
void Camera_Serialize();

// Extra Entity Functions


#endif //!OBJ_CAMERA_H
