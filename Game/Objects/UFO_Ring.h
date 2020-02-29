#ifndef OBJ_UFO_RING_H
#define OBJ_UFO_RING_H

#include "../../SonicMania.h"

//Object Class
class ObjectUFO_Ring : public Object {
public:

};

//Entity Class
class EntityUFO_Ring : public Entity {
public:

};

//Object Entity
ObjectUFO_Ring UFO_Ring;

//Entity Functions
void UFO_Ring_Update();
void UFO_Ring_EarlyUpdate();
void UFO_Ring_LateUpdate();
void UFO_Ring_Draw();
void UFO_Ring_Setup(void* subtype);
void UFO_Ring_StageLoad();
void UFO_Ring_GetAttributes();

#endif //!OBJ_UFO_RING_H
