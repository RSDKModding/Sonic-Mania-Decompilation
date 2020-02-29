#ifndef OBJ_HATCH_H
#define OBJ_HATCH_H

#include "../../SonicMania.h"

//Object Class
class ObjectHatch : public Object {
public:

};

//Entity Class
class EntityHatch : public Entity {
public:

};

//Object Entity
ObjectHatch Hatch;

//Entity Functions
void Hatch_Update();
void Hatch_EarlyUpdate();
void Hatch_LateUpdate();
void Hatch_Draw();
void Hatch_Setup(void* subtype);
void Hatch_StageLoad();
void Hatch_GetAttributes();

#endif //!OBJ_HATCH_H
