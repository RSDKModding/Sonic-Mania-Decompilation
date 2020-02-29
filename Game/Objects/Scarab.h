#ifndef OBJ_SCARAB_H
#define OBJ_SCARAB_H

//Object Class
class Scarab : Object {

};

//Entity Class
class EntityScarab : Entity {

};

//Entity Functions
void Scarab_Update();
void Scarab_EarlyUpdate();
void Scarab_LateUpdate();
void Scarab_Draw();
void Scarab_Setup(void* subtype);
void Scarab_StageLoad();
void Scarab_GetAttributes();

#endif //!OBJ_SCARAB_H
