#ifndef OBJ_FROSTTHROWER_H
#define OBJ_FROSTTHROWER_H

#include "../../SonicMania.h"

//Object Class
class ObjectFrostThrower : public Object {
public:

};

//Entity Class
class EntityFrostThrower : public Entity {
public:

};

//Object Entity
ObjectFrostThrower FrostThrower;

//Entity Functions
void FrostThrower_Update();
void FrostThrower_EarlyUpdate();
void FrostThrower_LateUpdate();
void FrostThrower_Draw();
void FrostThrower_Setup(void* subtype);
void FrostThrower_StageLoad();
void FrostThrower_GetAttributes();

#endif //!OBJ_FROSTTHROWER_H
