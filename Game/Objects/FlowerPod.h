#ifndef OBJ_FLOWERPOD_H
#define OBJ_FLOWERPOD_H

#include "../../SonicMania.h"

//Object Class
class ObjectFlowerPod : public Object {
public:

};

//Entity Class
class EntityFlowerPod : public Entity {
public:

};

//Object Entity
ObjectFlowerPod FlowerPod;

//Entity Functions
void FlowerPod_Update();
void FlowerPod_EarlyUpdate();
void FlowerPod_LateUpdate();
void FlowerPod_Draw();
void FlowerPod_Setup(void* subtype);
void FlowerPod_StageLoad();
void FlowerPod_GetAttributes();

#endif //!OBJ_FLOWERPOD_H
