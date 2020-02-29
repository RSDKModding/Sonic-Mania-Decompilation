#ifndef OBJ_GRABBER_H
#define OBJ_GRABBER_H

#include "../../SonicMania.h"

//Object Class
class ObjectGrabber : public Object {
public:

};

//Entity Class
class EntityGrabber : public Entity {
public:

};

//Object Entity
ObjectGrabber Grabber;

//Entity Functions
void Grabber_Update();
void Grabber_EarlyUpdate();
void Grabber_LateUpdate();
void Grabber_Draw();
void Grabber_Setup(void* subtype);
void Grabber_StageLoad();
void Grabber_GetAttributes();

#endif //!OBJ_GRABBER_H
