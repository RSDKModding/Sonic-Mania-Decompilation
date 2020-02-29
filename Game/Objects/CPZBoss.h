#ifndef OBJ_CPZBOSS_H
#define OBJ_CPZBOSS_H

#include "../../SonicMania.h"

//Object Class
class ObjectCPZBoss : public Object {
public:

};

//Entity Class
class EntityCPZBoss : public Entity {
public:

};

//Object Entity
ObjectCPZBoss CPZBoss;

//Entity Functions
void CPZBoss_Update();
void CPZBoss_EarlyUpdate();
void CPZBoss_LateUpdate();
void CPZBoss_Draw();
void CPZBoss_Setup(void* subtype);
void CPZBoss_StageLoad();
void CPZBoss_GetAttributes();

#endif //!OBJ_CPZBOSS_H
