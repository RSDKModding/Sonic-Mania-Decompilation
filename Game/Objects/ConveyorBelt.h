#ifndef OBJ_CONVEYORBELT_H
#define OBJ_CONVEYORBELT_H

#include "../../SonicMania.h"

//Object Class
class ObjectConveyorBelt : public Object {
public:

};

//Entity Class
class EntityConveyorBelt : public Entity {
public:

};

//Object Entity
ObjectConveyorBelt ConveyorBelt;

//Entity Functions
void ConveyorBelt_Update();
void ConveyorBelt_EarlyUpdate();
void ConveyorBelt_LateUpdate();
void ConveyorBelt_Draw();
void ConveyorBelt_Setup(void* subtype);
void ConveyorBelt_StageLoad();
void ConveyorBelt_GetAttributes();

#endif //!OBJ_CONVEYORBELT_H
