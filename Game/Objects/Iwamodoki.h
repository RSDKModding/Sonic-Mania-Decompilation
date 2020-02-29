#ifndef OBJ_IWAMODOKI_H
#define OBJ_IWAMODOKI_H

#include "../../SonicMania.h"

//Object Class
class ObjectIwamodoki : public Object {
public:

};

//Entity Class
class EntityIwamodoki : public Entity {
public:

};

//Object Entity
ObjectIwamodoki Iwamodoki;

//Entity Functions
void Iwamodoki_Update();
void Iwamodoki_EarlyUpdate();
void Iwamodoki_LateUpdate();
void Iwamodoki_Draw();
void Iwamodoki_Setup(void* subtype);
void Iwamodoki_StageLoad();
void Iwamodoki_GetAttributes();

#endif //!OBJ_IWAMODOKI_H
