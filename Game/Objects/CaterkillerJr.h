#ifndef OBJ_CATERKILLERJR_H
#define OBJ_CATERKILLERJR_H

#include "../../SonicMania.h"

//Object Class
class ObjectCaterkillerJr : public Object {
public:

};

//Entity Class
class EntityCaterkillerJr : public Entity {
public:

};

//Object Entity
ObjectCaterkillerJr CaterkillerJr;

//Entity Functions
void CaterkillerJr_Update();
void CaterkillerJr_EarlyUpdate();
void CaterkillerJr_LateUpdate();
void CaterkillerJr_Draw();
void CaterkillerJr_Setup(void* subtype);
void CaterkillerJr_StageLoad();
void CaterkillerJr_GetAttributes();

#endif //!OBJ_CATERKILLERJR_H
