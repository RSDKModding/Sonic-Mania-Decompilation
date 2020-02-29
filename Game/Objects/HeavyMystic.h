#ifndef OBJ_HEAVYMYSTIC_H
#define OBJ_HEAVYMYSTIC_H

//Object Class
class HeavyMystic : Object {

};

//Entity Class
class EntityHeavyMystic : Entity {

};

//Entity Functions
void HeavyMystic_Update();
void HeavyMystic_EarlyUpdate();
void HeavyMystic_LateUpdate();
void HeavyMystic_Draw();
void HeavyMystic_Setup(void* subtype);
void HeavyMystic_StageLoad();
void HeavyMystic_GetAttributes();

#endif //!OBJ_HEAVYMYSTIC_H
