#ifndef OBJ_SWEEP_H
#define OBJ_SWEEP_H

#include "../../SonicMania.h"

//Object Class
class ObjectSweep : public Object {
public:

};

//Entity Class
class EntitySweep : public Entity {
public:

};

//Object Entity
ObjectSweep Sweep;

//Entity Functions
void Sweep_Update();
void Sweep_EarlyUpdate();
void Sweep_LateUpdate();
void Sweep_Draw();
void Sweep_Setup(void* subtype);
void Sweep_StageLoad();
void Sweep_GetAttributes();

#endif //!OBJ_SWEEP_H
