#ifndef OBJ_CPZBOSS_H
#define OBJ_CPZBOSS_H

//Object Class
class CPZBoss : Object {

};

//Entity Class
class EntityCPZBoss : Entity {

};

//Entity Functions
void CPZBoss_Update();
void CPZBoss_EarlyUpdate();
void CPZBoss_LateUpdate();
void CPZBoss_Draw();
void CPZBoss_Setup(void* subtype);
void CPZBoss_StageLoad();
void CPZBoss_GetAttributes();

#endif //!OBJ_CPZBOSS_H
