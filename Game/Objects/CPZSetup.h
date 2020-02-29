#ifndef OBJ_CPZSETUP_H
#define OBJ_CPZSETUP_H

//Object Class
class CPZSetup : Object {

};

//Entity Class
class EntityCPZSetup : Entity {

};

//Entity Functions
void CPZSetup_Update();
void CPZSetup_EarlyUpdate();
void CPZSetup_LateUpdate();
void CPZSetup_Draw();
void CPZSetup_Setup(void* subtype);
void CPZSetup_StageLoad();
void CPZSetup_GetAttributes();

#endif //!OBJ_CPZSETUP_H
