#ifndef OBJ_CLUCKER_H
#define OBJ_CLUCKER_H

#include "../../SonicMania.h"

//Object Class
class ObjectClucker : public Object {
public:

};

//Entity Class
class EntityClucker : public Entity {
public:

};

//Object Entity
ObjectClucker Clucker;

//Entity Functions
void Clucker_Update();
void Clucker_EarlyUpdate();
void Clucker_LateUpdate();
void Clucker_Draw();
void Clucker_Setup(void* subtype);
void Clucker_StageLoad();
void Clucker_GetAttributes();

#endif //!OBJ_CLUCKER_H
