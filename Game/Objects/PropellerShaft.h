#ifndef OBJ_PROPELLERSHAFT_H
#define OBJ_PROPELLERSHAFT_H

#include "../../SonicMania.h"

//Object Class
class ObjectPropellerShaft : public Object {
public:

};

//Entity Class
class EntityPropellerShaft : public Entity {
public:

};

//Object Entity
ObjectPropellerShaft PropellerShaft;

//Entity Functions
void PropellerShaft_Update();
void PropellerShaft_EarlyUpdate();
void PropellerShaft_LateUpdate();
void PropellerShaft_Draw();
void PropellerShaft_Setup(void* subtype);
void PropellerShaft_StageLoad();
void PropellerShaft_GetAttributes();

#endif //!OBJ_PROPELLERSHAFT_H
