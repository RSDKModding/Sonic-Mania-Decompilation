#ifndef OBJ_ERZSETUP_H
#define OBJ_ERZSETUP_H

//Object Class
class ERZSetup : Object {

};

//Entity Class
class EntityERZSetup : Entity {

};

//Entity Functions
void ERZSetup_Update();
void ERZSetup_EarlyUpdate();
void ERZSetup_LateUpdate();
void ERZSetup_Draw();
void ERZSetup_Setup(void* subtype);
void ERZSetup_StageLoad();
void ERZSetup_GetAttributes();

#endif //!OBJ_ERZSETUP_H
