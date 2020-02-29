#ifndef OBJ_NEWTRON_H
#define OBJ_NEWTRON_H

//Object Class
class Newtron : Object {

};

//Entity Class
class EntityNewtron : Entity {

};

//Entity Functions
void Newtron_Update();
void Newtron_EarlyUpdate();
void Newtron_LateUpdate();
void Newtron_Draw();
void Newtron_Setup(void* subtype);
void Newtron_StageLoad();
void Newtron_GetAttributes();

#endif //!OBJ_NEWTRON_H
