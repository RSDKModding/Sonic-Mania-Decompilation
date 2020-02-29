#ifndef OBJ_UFO_RING_H
#define OBJ_UFO_RING_H

//Object Class
class UFO_Ring : Object {

};

//Entity Class
class EntityUFO_Ring : Entity {

};

//Entity Functions
void UFO_Ring_Update();
void UFO_Ring_EarlyUpdate();
void UFO_Ring_LateUpdate();
void UFO_Ring_Draw();
void UFO_Ring_Setup(void* subtype);
void UFO_Ring_StageLoad();
void UFO_Ring_GetAttributes();

#endif //!OBJ_UFO_RING_H
