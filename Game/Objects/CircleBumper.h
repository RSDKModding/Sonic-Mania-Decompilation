#ifndef OBJ_CIRCLEBUMPER_H
#define OBJ_CIRCLEBUMPER_H

#include "../../SonicMania.h"

//Object Class
class ObjectCircleBumper : public Object {
public:

};

//Entity Class
class EntityCircleBumper : public Entity {
public:

};

//Object Entity
ObjectCircleBumper CircleBumper;

//Entity Functions
void CircleBumper_Update();
void CircleBumper_EarlyUpdate();
void CircleBumper_LateUpdate();
void CircleBumper_Draw();
void CircleBumper_Setup(void* subtype);
void CircleBumper_StageLoad();
void CircleBumper_GetAttributes();

#endif //!OBJ_CIRCLEBUMPER_H
