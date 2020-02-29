#ifndef OBJ_BLOOMINATOR_H
#define OBJ_BLOOMINATOR_H

//Object Class
class Bloominator : Object {

};

//Entity Class
class EntityBloominator : Entity {

};

//Entity Functions
void Bloominator_Update();
void Bloominator_EarlyUpdate();
void Bloominator_LateUpdate();
void Bloominator_Draw();
void Bloominator_Setup(void* subtype);
void Bloominator_StageLoad();
void Bloominator_GetAttributes();

#endif //!OBJ_BLOOMINATOR_H
