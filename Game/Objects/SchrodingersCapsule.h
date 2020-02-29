#ifndef OBJ_SCHRODINGERSCAPSULE_H
#define OBJ_SCHRODINGERSCAPSULE_H

//Object Class
class SchrodingersCapsule : Object {

};

//Entity Class
class EntitySchrodingersCapsule : Entity {

};

//Entity Functions
void SchrodingersCapsule_Update();
void SchrodingersCapsule_EarlyUpdate();
void SchrodingersCapsule_LateUpdate();
void SchrodingersCapsule_Draw();
void SchrodingersCapsule_Setup(void* subtype);
void SchrodingersCapsule_StageLoad();
void SchrodingersCapsule_GetAttributes();

#endif //!OBJ_SCHRODINGERSCAPSULE_H
