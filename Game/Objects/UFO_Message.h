#ifndef OBJ_UFO_MESSAGE_H
#define OBJ_UFO_MESSAGE_H

//Object Class
class UFO_Message : Object {

};

//Entity Class
class EntityUFO_Message : Entity {

};

//Entity Functions
void UFO_Message_Update();
void UFO_Message_EarlyUpdate();
void UFO_Message_LateUpdate();
void UFO_Message_Draw();
void UFO_Message_Setup(void* subtype);
void UFO_Message_StageLoad();
void UFO_Message_GetAttributes();

#endif //!OBJ_UFO_MESSAGE_H
