#ifndef OBJ_SPZ2SETUP_H
#define OBJ_SPZ2SETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectSPZ2Setup : public Object {
public:

};

//Entity Class
class EntitySPZ2Setup : public Entity {
public:

};

//Object Entity
ObjectSPZ2Setup SPZ2Setup;

//Entity Functions
void SPZ2Setup_Update();
void SPZ2Setup_EarlyUpdate();
void SPZ2Setup_LateUpdate();
void SPZ2Setup_Draw();
void SPZ2Setup_Setup(void* subtype);
void SPZ2Setup_StageLoad();
void SPZ2Setup_GetAttributes();

#endif //!OBJ_SPZ2SETUP_H
