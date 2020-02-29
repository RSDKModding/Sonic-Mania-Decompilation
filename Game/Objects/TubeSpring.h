#ifndef OBJ_TUBESPRING_H
#define OBJ_TUBESPRING_H

#include "../../SonicMania.h"

//Object Class
class ObjectTubeSpring : public Object {
public:

};

//Entity Class
class EntityTubeSpring : public Entity {
public:

};

//Object Entity
ObjectTubeSpring TubeSpring;

//Entity Functions
void TubeSpring_Update();
void TubeSpring_EarlyUpdate();
void TubeSpring_LateUpdate();
void TubeSpring_Draw();
void TubeSpring_Setup(void* subtype);
void TubeSpring_StageLoad();
void TubeSpring_GetAttributes();

#endif //!OBJ_TUBESPRING_H
