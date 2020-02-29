#ifndef OBJ_DANGO_H
#define OBJ_DANGO_H

//Object Class
class Dango : Object {

};

//Entity Class
class EntityDango : Entity {

};

//Entity Functions
void Dango_Update();
void Dango_EarlyUpdate();
void Dango_LateUpdate();
void Dango_Draw();
void Dango_Setup(void* subtype);
void Dango_StageLoad();
void Dango_GetAttributes();

#endif //!OBJ_DANGO_H
