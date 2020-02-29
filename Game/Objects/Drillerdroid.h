#ifndef OBJ_DRILLERDROID_H
#define OBJ_DRILLERDROID_H

//Object Class
class Drillerdroid : Object {

};

//Entity Class
class EntityDrillerdroid : Entity {

};

//Entity Functions
void Drillerdroid_Update();
void Drillerdroid_EarlyUpdate();
void Drillerdroid_LateUpdate();
void Drillerdroid_Draw();
void Drillerdroid_Setup(void* subtype);
void Drillerdroid_StageLoad();
void Drillerdroid_GetAttributes();

#endif //!OBJ_DRILLERDROID_H
