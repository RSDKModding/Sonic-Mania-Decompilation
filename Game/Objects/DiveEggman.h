#ifndef OBJ_DIVEEGGMAN_H
#define OBJ_DIVEEGGMAN_H

//Object Class
class DiveEggman : Object {

};

//Entity Class
class EntityDiveEggman : Entity {

};

//Entity Functions
void DiveEggman_Update();
void DiveEggman_EarlyUpdate();
void DiveEggman_LateUpdate();
void DiveEggman_Draw();
void DiveEggman_Setup(void* subtype);
void DiveEggman_StageLoad();
void DiveEggman_GetAttributes();

#endif //!OBJ_DIVEEGGMAN_H
