#ifndef OBJ_TVFLYINGBATTERY_H
#define OBJ_TVFLYINGBATTERY_H

#include "../../SonicMania.h"

//Object Class
class ObjectTVFlyingBattery : public Object {
public:

};

//Entity Class
class EntityTVFlyingBattery : public Entity {
public:

};

//Object Entity
ObjectTVFlyingBattery TVFlyingBattery;

//Entity Functions
void TVFlyingBattery_Update();
void TVFlyingBattery_EarlyUpdate();
void TVFlyingBattery_LateUpdate();
void TVFlyingBattery_Draw();
void TVFlyingBattery_Setup(void* subtype);
void TVFlyingBattery_StageLoad();
void TVFlyingBattery_GetAttributes();

#endif //!OBJ_TVFLYINGBATTERY_H
