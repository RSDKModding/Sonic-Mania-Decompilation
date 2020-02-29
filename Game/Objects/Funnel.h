#ifndef OBJ_FUNNEL_H
#define OBJ_FUNNEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectFunnel : public Object {
public:

};

//Entity Class
class EntityFunnel : public Entity {
public:

};

//Object Entity
ObjectFunnel Funnel;

//Entity Functions
void Funnel_Update();
void Funnel_EarlyUpdate();
void Funnel_LateUpdate();
void Funnel_Draw();
void Funnel_Setup(void* subtype);
void Funnel_StageLoad();
void Funnel_GetAttributes();

#endif //!OBJ_FUNNEL_H
