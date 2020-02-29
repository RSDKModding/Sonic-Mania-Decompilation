#ifndef OBJ_UFO_CAMERA_H
#define OBJ_UFO_CAMERA_H

//Object Class
class UFO_Camera : Object {

};

//Entity Class
class EntityUFO_Camera : Entity {

};

//Entity Functions
void UFO_Camera_Update();
void UFO_Camera_EarlyUpdate();
void UFO_Camera_LateUpdate();
void UFO_Camera_Draw();
void UFO_Camera_Setup(void* subtype);
void UFO_Camera_StageLoad();
void UFO_Camera_GetAttributes();

#endif //!OBJ_UFO_CAMERA_H
