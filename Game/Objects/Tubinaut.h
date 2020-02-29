#ifndef OBJ_TUBINAUT_H
#define OBJ_TUBINAUT_H

#include "../../SonicMania.h"

//Object Class
class ObjectTubinaut : public Object {
public:

};

//Entity Class
class EntityTubinaut : public Entity {
public:

};

//Object Entity
ObjectTubinaut Tubinaut;

//Entity Functions
void Tubinaut_Update();
void Tubinaut_EarlyUpdate();
void Tubinaut_LateUpdate();
void Tubinaut_Draw();
void Tubinaut_Setup(void* subtype);
void Tubinaut_StageLoad();
void Tubinaut_GetAttributes();

#endif //!OBJ_TUBINAUT_H
