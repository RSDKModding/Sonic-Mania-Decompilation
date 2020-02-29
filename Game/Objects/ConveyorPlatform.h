#ifndef OBJ_CONVEYORPLATFORM_H
#define OBJ_CONVEYORPLATFORM_H

//Object Class
class ConveyorPlatform : Object {

};

//Entity Class
class EntityConveyorPlatform : Entity {

};

//Entity Functions
void ConveyorPlatform_Update();
void ConveyorPlatform_EarlyUpdate();
void ConveyorPlatform_LateUpdate();
void ConveyorPlatform_Draw();
void ConveyorPlatform_Setup(void* subtype);
void ConveyorPlatform_StageLoad();
void ConveyorPlatform_GetAttributes();

#endif //!OBJ_CONVEYORPLATFORM_H
