#ifndef OBJ_VULTRON_H
#define OBJ_VULTRON_H

//Object Class
class Vultron : Object {

};

//Entity Class
class EntityVultron : Entity {

};

//Entity Functions
void Vultron_Update();
void Vultron_EarlyUpdate();
void Vultron_LateUpdate();
void Vultron_Draw();
void Vultron_Setup(void* subtype);
void Vultron_StageLoad();
void Vultron_GetAttributes();

#endif //!OBJ_VULTRON_H
