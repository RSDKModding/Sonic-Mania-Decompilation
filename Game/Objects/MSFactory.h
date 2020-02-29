#ifndef OBJ_MSFACTORY_H
#define OBJ_MSFACTORY_H

//Object Class
class MSFactory : Object {

};

//Entity Class
class EntityMSFactory : Entity {

};

//Entity Functions
void MSFactory_Update();
void MSFactory_EarlyUpdate();
void MSFactory_LateUpdate();
void MSFactory_Draw();
void MSFactory_Setup(void* subtype);
void MSFactory_StageLoad();
void MSFactory_GetAttributes();

#endif //!OBJ_MSFACTORY_H
