#ifndef OBJ_FLAMESPRING_H
#define OBJ_FLAMESPRING_H

#include "../../SonicMania.h"

//Object Class
class ObjectFlameSpring : public Object {
public:

};

//Entity Class
class EntityFlameSpring : public Entity {
public:

};

//Object Entity
ObjectFlameSpring FlameSpring;

//Entity Functions
void FlameSpring_Update();
void FlameSpring_EarlyUpdate();
void FlameSpring_LateUpdate();
void FlameSpring_Draw();
void FlameSpring_Setup(void* subtype);
void FlameSpring_StageLoad();
void FlameSpring_GetAttributes();

#endif //!OBJ_FLAMESPRING_H
