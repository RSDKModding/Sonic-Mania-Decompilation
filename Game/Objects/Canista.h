#ifndef OBJ_CANISTA_H
#define OBJ_CANISTA_H

#include "../../SonicMania.h"

//Object Class
class ObjectCanista : public Object {
public:

};

//Entity Class
class EntityCanista : public Entity {
public:

};

//Object Entity
ObjectCanista Canista;

//Entity Functions
void Canista_Update();
void Canista_EarlyUpdate();
void Canista_LateUpdate();
void Canista_Draw();
void Canista_Setup(void* subtype);
void Canista_StageLoad();
void Canista_GetAttributes();

#endif //!OBJ_CANISTA_H
