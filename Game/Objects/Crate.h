#ifndef OBJ_CRATE_H
#define OBJ_CRATE_H

//Object Class
class Crate : Object {

};

//Entity Class
class EntityCrate : Entity {

};

//Entity Functions
void Crate_Update();
void Crate_EarlyUpdate();
void Crate_LateUpdate();
void Crate_Draw();
void Crate_Setup(void* subtype);
void Crate_StageLoad();
void Crate_GetAttributes();

#endif //!OBJ_CRATE_H
