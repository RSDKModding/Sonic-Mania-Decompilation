#ifndef OBJ_CONVEYORWHEEL_H
#define OBJ_CONVEYORWHEEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectConveyorWheel : public Object {
public:

};

//Entity Class
class EntityConveyorWheel : public Entity {
public:

};

//Object Entity
ObjectConveyorWheel ConveyorWheel;

//Entity Functions
void ConveyorWheel_Update();
void ConveyorWheel_EarlyUpdate();
void ConveyorWheel_LateUpdate();
void ConveyorWheel_Draw();
void ConveyorWheel_Setup(void* subtype);
void ConveyorWheel_StageLoad();
void ConveyorWheel_GetAttributes();

#endif //!OBJ_CONVEYORWHEEL_H
