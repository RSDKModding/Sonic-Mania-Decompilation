#ifndef OBJ_SP500_H
#define OBJ_SP500_H

#include "../../SonicMania.h"

//Object Class
class ObjectSP500 : public Object {
public:

};

//Entity Class
class EntitySP500 : public Entity {
public:

};

//Object Entity
ObjectSP500 SP500;

//Entity Functions
void SP500_Update();
void SP500_EarlyUpdate();
void SP500_LateUpdate();
void SP500_Draw();
void SP500_Setup(void* subtype);
void SP500_StageLoad();
void SP500_GetAttributes();

#endif //!OBJ_SP500_H
