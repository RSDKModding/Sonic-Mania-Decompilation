#ifndef OBJ_UFO_SETUP_H
#define OBJ_UFO_SETUP_H

//Object Class
class UFO_Setup : Object {

};

//Entity Class
class EntityUFO_Setup : Entity {

};

//Entity Functions
void UFO_Setup_Update();
void UFO_Setup_EarlyUpdate();
void UFO_Setup_LateUpdate();
void UFO_Setup_Draw();
void UFO_Setup_Setup(void* subtype);
void UFO_Setup_StageLoad();
void UFO_Setup_GetAttributes();

#endif //!OBJ_UFO_SETUP_H
