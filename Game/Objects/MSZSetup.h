#ifndef OBJ_MSZSETUP_H
#define OBJ_MSZSETUP_H

//Object Class
class MSZSetup : Object {

};

//Entity Class
class EntityMSZSetup : Entity {

};

//Entity Functions
void MSZSetup_Update();
void MSZSetup_EarlyUpdate();
void MSZSetup_LateUpdate();
void MSZSetup_Draw();
void MSZSetup_Setup(void* subtype);
void MSZSetup_StageLoad();
void MSZSetup_GetAttributes();

#endif //!OBJ_MSZSETUP_H
