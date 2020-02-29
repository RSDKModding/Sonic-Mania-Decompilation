#ifndef OBJ_TVFLYINGBATTERY_H
#define OBJ_TVFLYINGBATTERY_H

//Object Class
class TVFlyingBattery : Object {

};

//Entity Class
class EntityTVFlyingBattery : Entity {

};

//Entity Functions
void TVFlyingBattery_Update();
void TVFlyingBattery_EarlyUpdate();
void TVFlyingBattery_LateUpdate();
void TVFlyingBattery_Draw();
void TVFlyingBattery_Setup(void* subtype);
void TVFlyingBattery_StageLoad();
void TVFlyingBattery_GetAttributes();

#endif //!OBJ_TVFLYINGBATTERY_H
