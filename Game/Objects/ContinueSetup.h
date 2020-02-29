#ifndef OBJ_CONTINUESETUP_H
#define OBJ_CONTINUESETUP_H

//Object Class
class ContinueSetup : Object {

};

//Entity Class
class EntityContinueSetup : Entity {

};

//Entity Functions
void ContinueSetup_Update();
void ContinueSetup_EarlyUpdate();
void ContinueSetup_LateUpdate();
void ContinueSetup_Draw();
void ContinueSetup_Setup(void* subtype);
void ContinueSetup_StageLoad();
void ContinueSetup_GetAttributes();

#endif //!OBJ_CONTINUESETUP_H
