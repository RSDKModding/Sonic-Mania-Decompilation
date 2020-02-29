#ifndef OBJ_BUGGERNAUT_H
#define OBJ_BUGGERNAUT_H

//Object Class
class Buggernaut : Object {

};

//Entity Class
class EntityBuggernaut : Entity {

};

//Entity Functions
void Buggernaut_Update();
void Buggernaut_EarlyUpdate();
void Buggernaut_LateUpdate();
void Buggernaut_Draw();
void Buggernaut_Setup(void* subtype);
void Buggernaut_StageLoad();
void Buggernaut_GetAttributes();

#endif //!OBJ_BUGGERNAUT_H
