#ifndef OBJ_FLAMETHROWER_H
#define OBJ_FLAMETHROWER_H

#include "../../SonicMania.h"

//Object Class
class ObjectFlamethrower : public Object {
public:

};

//Entity Class
class EntityFlamethrower : public Entity {
public:

};

//Object Entity
ObjectFlamethrower Flamethrower;

//Entity Functions
void Flamethrower_Update();
void Flamethrower_EarlyUpdate();
void Flamethrower_LateUpdate();
void Flamethrower_Draw();
void Flamethrower_Setup(void* subtype);
void Flamethrower_StageLoad();
void Flamethrower_GetAttributes();

#endif //!OBJ_FLAMETHROWER_H
