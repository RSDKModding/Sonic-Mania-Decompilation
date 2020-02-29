#ifndef OBJ_DDWRECKER_H
#define OBJ_DDWRECKER_H

//Object Class
class DDWrecker : Object {

};

//Entity Class
class EntityDDWrecker : Entity {

};

//Entity Functions
void DDWrecker_Update();
void DDWrecker_EarlyUpdate();
void DDWrecker_LateUpdate();
void DDWrecker_Draw();
void DDWrecker_Setup(void* subtype);
void DDWrecker_StageLoad();
void DDWrecker_GetAttributes();

#endif //!OBJ_DDWRECKER_H
