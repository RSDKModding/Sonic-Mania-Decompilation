#ifndef OBJ_CONVEYORWHEEL_H
#define OBJ_CONVEYORWHEEL_H

//Object Class
class ConveyorWheel : Object {

};

//Entity Class
class EntityConveyorWheel : Entity {

};

//Entity Functions
void ConveyorWheel_Update();
void ConveyorWheel_EarlyUpdate();
void ConveyorWheel_LateUpdate();
void ConveyorWheel_Draw();
void ConveyorWheel_Setup(void* subtype);
void ConveyorWheel_StageLoad();
void ConveyorWheel_GetAttributes();

#endif //!OBJ_CONVEYORWHEEL_H
