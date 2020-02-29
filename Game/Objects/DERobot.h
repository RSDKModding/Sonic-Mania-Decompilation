#ifndef OBJ_DEROBOT_H
#define OBJ_DEROBOT_H

//Object Class
class DERobot : Object {

};

//Entity Class
class EntityDERobot : Entity {

};

//Entity Functions
void DERobot_Update();
void DERobot_EarlyUpdate();
void DERobot_LateUpdate();
void DERobot_Draw();
void DERobot_Setup(void* subtype);
void DERobot_StageLoad();
void DERobot_GetAttributes();

#endif //!OBJ_DEROBOT_H
