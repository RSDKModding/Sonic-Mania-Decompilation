#ifndef OBJ_TORNADO_H
#define OBJ_TORNADO_H

//Object Class
class Tornado : Object {

};

//Entity Class
class EntityTornado : Entity {

};

//Entity Functions
void Tornado_Update();
void Tornado_EarlyUpdate();
void Tornado_LateUpdate();
void Tornado_Draw();
void Tornado_Setup(void* subtype);
void Tornado_StageLoad();
void Tornado_GetAttributes();

#endif //!OBJ_TORNADO_H
