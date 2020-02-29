#ifndef OBJ_CANISTA_H
#define OBJ_CANISTA_H

//Object Class
class Canista : Object {

};

//Entity Class
class EntityCanista : Entity {

};

//Entity Functions
void Canista_Update();
void Canista_EarlyUpdate();
void Canista_LateUpdate();
void Canista_Draw();
void Canista_Setup(void* subtype);
void Canista_StageLoad();
void Canista_GetAttributes();

#endif //!OBJ_CANISTA_H
