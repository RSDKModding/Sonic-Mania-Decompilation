#ifndef OBJ_SPINY_H
#define OBJ_SPINY_H

#include "../../SonicMania.h"

//Object Class
class ObjectSpiny : public Object {
public:

};

//Entity Class
class EntitySpiny : public Entity {
public:

};

//Object Entity
ObjectSpiny Spiny;

//Entity Functions
void Spiny_Update();
void Spiny_EarlyUpdate();
void Spiny_LateUpdate();
void Spiny_Draw();
void Spiny_Setup(void* subtype);
void Spiny_StageLoad();
void Spiny_GetAttributes();

#endif //!OBJ_SPINY_H
