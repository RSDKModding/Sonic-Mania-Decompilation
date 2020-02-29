#ifndef OBJ_FLOWERPOD_H
#define OBJ_FLOWERPOD_H

//Object Class
class FlowerPod : Object {

};

//Entity Class
class EntityFlowerPod : Entity {

};

//Entity Functions
void FlowerPod_Update();
void FlowerPod_EarlyUpdate();
void FlowerPod_LateUpdate();
void FlowerPod_Draw();
void FlowerPod_Setup(void* subtype);
void FlowerPod_StageLoad();
void FlowerPod_GetAttributes();

#endif //!OBJ_FLOWERPOD_H
