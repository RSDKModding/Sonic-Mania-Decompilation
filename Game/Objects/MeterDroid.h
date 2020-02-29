#ifndef OBJ_METERDROID_H
#define OBJ_METERDROID_H

#include "../../SonicMania.h"

//Object Class
class ObjectMeterDroid : public Object {
public:

};

//Entity Class
class EntityMeterDroid : public Entity {
public:

};

//Object Entity
ObjectMeterDroid MeterDroid;

//Entity Functions
void MeterDroid_Update();
void MeterDroid_EarlyUpdate();
void MeterDroid_LateUpdate();
void MeterDroid_Draw();
void MeterDroid_Setup(void* subtype);
void MeterDroid_StageLoad();
void MeterDroid_GetAttributes();

#endif //!OBJ_METERDROID_H
