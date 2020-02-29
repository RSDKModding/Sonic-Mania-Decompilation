#ifndef OBJ_LOVETESTER_H
#define OBJ_LOVETESTER_H

//Object Class
class LoveTester : Object {

};

//Entity Class
class EntityLoveTester : Entity {

};

//Entity Functions
void LoveTester_Update();
void LoveTester_EarlyUpdate();
void LoveTester_LateUpdate();
void LoveTester_Draw();
void LoveTester_Setup(void* subtype);
void LoveTester_StageLoad();
void LoveTester_GetAttributes();

#endif //!OBJ_LOVETESTER_H
