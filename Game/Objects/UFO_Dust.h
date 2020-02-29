#ifndef OBJ_UFO_DUST_H
#define OBJ_UFO_DUST_H

#include "../../SonicMania.h"

//Object Class
class ObjectUFO_Dust : public Object {
public:

};

//Entity Class
class EntityUFO_Dust : public Entity {
public:

};

//Object Entity
ObjectUFO_Dust UFO_Dust;

//Entity Functions
void UFO_Dust_Update();
void UFO_Dust_EarlyUpdate();
void UFO_Dust_LateUpdate();
void UFO_Dust_Draw();
void UFO_Dust_Setup(void* subtype);
void UFO_Dust_StageLoad();
void UFO_Dust_GetAttributes();

#endif //!OBJ_UFO_DUST_H
