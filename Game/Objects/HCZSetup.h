#ifndef OBJ_HCZSETUP_H
#define OBJ_HCZSETUP_H

//Object Class
class HCZSetup : Object {

};

//Entity Class
class EntityHCZSetup : Entity {

};

//Entity Functions
void HCZSetup_Update();
void HCZSetup_EarlyUpdate();
void HCZSetup_LateUpdate();
void HCZSetup_Draw();
void HCZSetup_Setup(void* subtype);
void HCZSetup_StageLoad();
void HCZSetup_GetAttributes();

#endif //!OBJ_HCZSETUP_H
