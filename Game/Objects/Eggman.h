#ifndef OBJ_EGGMAN_H
#define OBJ_EGGMAN_H

//Object Class
class Eggman : Object {

};

//Entity Class
class EntityEggman : Entity {

};

//Entity Functions
void Eggman_Update();
void Eggman_EarlyUpdate();
void Eggman_LateUpdate();
void Eggman_Draw();
void Eggman_Setup(void* subtype);
void Eggman_StageLoad();
void Eggman_GetAttributes();

#endif //!OBJ_EGGMAN_H
