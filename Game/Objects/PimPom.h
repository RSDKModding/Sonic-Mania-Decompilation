#ifndef OBJ_PIMPOM_H
#define OBJ_PIMPOM_H

#include "../../SonicMania.h"

//Object Class
class ObjectPimPom : public Object {
public:

};

//Entity Class
class EntityPimPom : public Entity {
public:

};

//Object Entity
ObjectPimPom PimPom;

//Entity Functions
void PimPom_Update();
void PimPom_EarlyUpdate();
void PimPom_LateUpdate();
void PimPom_Draw();
void PimPom_Setup(void* subtype);
void PimPom_StageLoad();
void PimPom_GetAttributes();

#endif //!OBJ_PIMPOM_H
