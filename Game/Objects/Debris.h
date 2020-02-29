#ifndef OBJ_DEBRIS_H
#define OBJ_DEBRIS_H

#include "../../SonicMania.h"

//Object Class
class ObjectDebris : public Object {
public:

};

//Entity Class
class EntityDebris : public Entity {
public:

};

//Object Entity
ObjectDebris Debris;

//Entity Functions
void Debris_Update();
void Debris_EarlyUpdate();
void Debris_LateUpdate();
void Debris_Draw();
void Debris_Setup(void* subtype);
void Debris_StageLoad();
void Debris_GetAttributes();

#endif //!OBJ_DEBRIS_H
