#ifndef OBJ_SYRINGE_H
#define OBJ_SYRINGE_H

//Object Class
class Syringe : Object {

};

//Entity Class
class EntitySyringe : Entity {

};

//Entity Functions
void Syringe_Update();
void Syringe_EarlyUpdate();
void Syringe_LateUpdate();
void Syringe_Draw();
void Syringe_Setup(void* subtype);
void Syringe_StageLoad();
void Syringe_GetAttributes();

#endif //!OBJ_SYRINGE_H
