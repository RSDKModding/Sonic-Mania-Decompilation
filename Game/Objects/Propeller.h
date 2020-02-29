#ifndef OBJ_PROPELLER_H
#define OBJ_PROPELLER_H

//Object Class
class Propeller : Object {

};

//Entity Class
class EntityPropeller : Entity {

};

//Entity Functions
void Propeller_Update();
void Propeller_EarlyUpdate();
void Propeller_LateUpdate();
void Propeller_Draw();
void Propeller_Setup(void* subtype);
void Propeller_StageLoad();
void Propeller_GetAttributes();

#endif //!OBJ_PROPELLER_H
