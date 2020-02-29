#ifndef OBJ_DRILLERDROID_H
#define OBJ_DRILLERDROID_H

#include "../../SonicMania.h"

//Object Class
class ObjectDrillerdroid : public Object {
public:

};

//Entity Class
class EntityDrillerdroid : public Entity {
public:

};

//Object Entity
ObjectDrillerdroid Drillerdroid;

//Entity Functions
void Drillerdroid_Update();
void Drillerdroid_EarlyUpdate();
void Drillerdroid_LateUpdate();
void Drillerdroid_Draw();
void Drillerdroid_Setup(void* subtype);
void Drillerdroid_StageLoad();
void Drillerdroid_GetAttributes();

#endif //!OBJ_DRILLERDROID_H
