#ifndef OBJ_UFO_WATER_H
#define OBJ_UFO_WATER_H

//Object Class
class UFO_Water : Object {

};

//Entity Class
class EntityUFO_Water : Entity {

};

//Entity Functions
void UFO_Water_Update();
void UFO_Water_EarlyUpdate();
void UFO_Water_LateUpdate();
void UFO_Water_Draw();
void UFO_Water_Setup(void* subtype);
void UFO_Water_StageLoad();
void UFO_Water_GetAttributes();

#endif //!OBJ_UFO_WATER_H
