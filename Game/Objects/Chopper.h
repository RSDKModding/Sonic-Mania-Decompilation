#ifndef OBJ_CHOPPER_H
#define OBJ_CHOPPER_H

//Object Class
class Chopper : Object {

};

//Entity Class
class EntityChopper : Entity {

};

//Entity Functions
void Chopper_Update();
void Chopper_EarlyUpdate();
void Chopper_LateUpdate();
void Chopper_Draw();
void Chopper_Setup(void* subtype);
void Chopper_StageLoad();
void Chopper_GetAttributes();

#endif //!OBJ_CHOPPER_H
