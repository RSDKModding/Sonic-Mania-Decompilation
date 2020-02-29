#ifndef OBJ_SPEAR_H
#define OBJ_SPEAR_H

//Object Class
class Spear : Object {

};

//Entity Class
class EntitySpear : Entity {

};

//Entity Functions
void Spear_Update();
void Spear_EarlyUpdate();
void Spear_LateUpdate();
void Spear_Draw();
void Spear_Setup(void* subtype);
void Spear_StageLoad();
void Spear_GetAttributes();

#endif //!OBJ_SPEAR_H
