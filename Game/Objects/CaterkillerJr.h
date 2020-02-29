#ifndef OBJ_CATERKILLERJR_H
#define OBJ_CATERKILLERJR_H

//Object Class
class CaterkillerJr : Object {

};

//Entity Class
class EntityCaterkillerJr : Entity {

};

//Entity Functions
void CaterkillerJr_Update();
void CaterkillerJr_EarlyUpdate();
void CaterkillerJr_LateUpdate();
void CaterkillerJr_Draw();
void CaterkillerJr_Setup(void* subtype);
void CaterkillerJr_StageLoad();
void CaterkillerJr_GetAttributes();

#endif //!OBJ_CATERKILLERJR_H
