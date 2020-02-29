#ifndef OBJ_STALACTITE_H
#define OBJ_STALACTITE_H

#include "../../SonicMania.h"

//Object Class
class ObjectStalactite : public Object {
public:

};

//Entity Class
class EntityStalactite : public Entity {
public:

};

//Object Entity
ObjectStalactite Stalactite;

//Entity Functions
void Stalactite_Update();
void Stalactite_EarlyUpdate();
void Stalactite_LateUpdate();
void Stalactite_Draw();
void Stalactite_Setup(void* subtype);
void Stalactite_StageLoad();
void Stalactite_GetAttributes();

#endif //!OBJ_STALACTITE_H
