#ifndef OBJ_CONVEYORBELT_H
#define OBJ_CONVEYORBELT_H

//Object Class
class ConveyorBelt : Object {

};

//Entity Class
class EntityConveyorBelt : Entity {

};

//Entity Functions
void ConveyorBelt_Update();
void ConveyorBelt_EarlyUpdate();
void ConveyorBelt_LateUpdate();
void ConveyorBelt_Draw();
void ConveyorBelt_Setup(void* subtype);
void ConveyorBelt_StageLoad();
void ConveyorBelt_GetAttributes();

#endif //!OBJ_CONVEYORBELT_H
