#ifndef OBJ_GRABBER_H
#define OBJ_GRABBER_H

//Object Class
class Grabber : Object {

};

//Entity Class
class EntityGrabber : Entity {

};

//Entity Functions
void Grabber_Update();
void Grabber_EarlyUpdate();
void Grabber_LateUpdate();
void Grabber_Draw();
void Grabber_Setup(void* subtype);
void Grabber_StageLoad();
void Grabber_GetAttributes();

#endif //!OBJ_GRABBER_H
