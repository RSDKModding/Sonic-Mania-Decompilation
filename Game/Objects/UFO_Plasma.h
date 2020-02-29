#ifndef OBJ_UFO_PLASMA_H
#define OBJ_UFO_PLASMA_H

#include "../../SonicMania.h"

//Object Class
class ObjectUFO_Plasma : public Object {
public:

};

//Entity Class
class EntityUFO_Plasma : public Entity {
public:

};

//Object Entity
ObjectUFO_Plasma UFO_Plasma;

//Entity Functions
void UFO_Plasma_Update();
void UFO_Plasma_EarlyUpdate();
void UFO_Plasma_LateUpdate();
void UFO_Plasma_Draw();
void UFO_Plasma_Setup(void* subtype);
void UFO_Plasma_StageLoad();
void UFO_Plasma_GetAttributes();

#endif //!OBJ_UFO_PLASMA_H
