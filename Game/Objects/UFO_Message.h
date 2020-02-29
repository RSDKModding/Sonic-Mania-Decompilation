#ifndef OBJ_UFO_MESSAGE_H
#define OBJ_UFO_MESSAGE_H

#include "../../SonicMania.h"

//Object Class
class ObjectUFO_Message : public Object {
public:

};

//Entity Class
class EntityUFO_Message : public Entity {
public:

};

//Object Entity
ObjectUFO_Message UFO_Message;

//Entity Functions
void UFO_Message_Update();
void UFO_Message_EarlyUpdate();
void UFO_Message_LateUpdate();
void UFO_Message_Draw();
void UFO_Message_Setup(void* subtype);
void UFO_Message_StageLoad();
void UFO_Message_GetAttributes();

#endif //!OBJ_UFO_MESSAGE_H
