#ifndef OBJ_OCTUS_H
#define OBJ_OCTUS_H

//Object Class
class Octus : Object {

};

//Entity Class
class EntityOctus : Entity {

};

//Entity Functions
void Octus_Update();
void Octus_EarlyUpdate();
void Octus_LateUpdate();
void Octus_Draw();
void Octus_Setup(void* subtype);
void Octus_StageLoad();
void Octus_GetAttributes();

#endif //!OBJ_OCTUS_H
