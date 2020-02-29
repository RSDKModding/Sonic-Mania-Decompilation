#ifndef OBJ_METERDROID_H
#define OBJ_METERDROID_H

//Object Class
class MeterDroid : Object {

};

//Entity Class
class EntityMeterDroid : Entity {

};

//Entity Functions
void MeterDroid_Update();
void MeterDroid_EarlyUpdate();
void MeterDroid_LateUpdate();
void MeterDroid_Draw();
void MeterDroid_Setup(void* subtype);
void MeterDroid_StageLoad();
void MeterDroid_GetAttributes();

#endif //!OBJ_METERDROID_H
