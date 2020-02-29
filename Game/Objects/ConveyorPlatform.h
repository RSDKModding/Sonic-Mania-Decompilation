#ifndef OBJ_CONVEYORPLATFORM_H
#define OBJ_CONVEYORPLATFORM_H

#include "../../SonicMania.h"

//Object Class
class ObjectConveyorPlatform : public Object {
public:

};

//Entity Class
class EntityConveyorPlatform : public Entity {
public:

};

//Object Entity
ObjectConveyorPlatform ConveyorPlatform;

//Entity Functions
void ConveyorPlatform_Update();
void ConveyorPlatform_EarlyUpdate();
void ConveyorPlatform_LateUpdate();
void ConveyorPlatform_Draw();
void ConveyorPlatform_Setup(void* subtype);
void ConveyorPlatform_StageLoad();
void ConveyorPlatform_GetAttributes();

#endif //!OBJ_CONVEYORPLATFORM_H
