#ifndef OBJ_HEAVYRIDER_H
#define OBJ_HEAVYRIDER_H

//Object Class
class HeavyRider : Object {

};

//Entity Class
class EntityHeavyRider : Entity {

};

//Entity Functions
void HeavyRider_Update();
void HeavyRider_EarlyUpdate();
void HeavyRider_LateUpdate();
void HeavyRider_Draw();
void HeavyRider_Setup(void* subtype);
void HeavyRider_StageLoad();
void HeavyRider_GetAttributes();

#endif //!OBJ_HEAVYRIDER_H
