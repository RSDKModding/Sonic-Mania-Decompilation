#ifndef OBJ_NEWTRON_H
#define OBJ_NEWTRON_H

#include "../../SonicMania.h"

//Object Class
class ObjectNewtron : public Object {
public:

};

//Entity Class
class EntityNewtron : public Entity {
public:

};

//Object Entity
ObjectNewtron Newtron;

//Entity Functions
void Newtron_Update();
void Newtron_EarlyUpdate();
void Newtron_LateUpdate();
void Newtron_Draw();
void Newtron_Setup(void* subtype);
void Newtron_StageLoad();
void Newtron_GetAttributes();

#endif //!OBJ_NEWTRON_H
