#ifndef OBJ_UFO_SHADOW_H
#define OBJ_UFO_SHADOW_H

#include "../../SonicMania.h"

//Object Class
class ObjectUFO_Shadow : public Object {
public:

};

//Entity Class
class EntityUFO_Shadow : public Entity {
public:

};

//Object Entity
ObjectUFO_Shadow UFO_Shadow;

//Entity Functions
void UFO_Shadow_Update();
void UFO_Shadow_EarlyUpdate();
void UFO_Shadow_LateUpdate();
void UFO_Shadow_Draw();
void UFO_Shadow_Setup(void* subtype);
void UFO_Shadow_StageLoad();
void UFO_Shadow_GetAttributes();

#endif //!OBJ_UFO_SHADOW_H
