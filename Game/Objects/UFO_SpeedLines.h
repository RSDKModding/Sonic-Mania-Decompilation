#ifndef OBJ_UFO_SPEEDLINES_H
#define OBJ_UFO_SPEEDLINES_H

//Object Class
class UFO_SpeedLines : Object {

};

//Entity Class
class EntityUFO_SpeedLines : Entity {

};

//Entity Functions
void UFO_SpeedLines_Update();
void UFO_SpeedLines_EarlyUpdate();
void UFO_SpeedLines_LateUpdate();
void UFO_SpeedLines_Draw();
void UFO_SpeedLines_Setup(void* subtype);
void UFO_SpeedLines_StageLoad();
void UFO_SpeedLines_GetAttributes();

#endif //!OBJ_UFO_SPEEDLINES_H
