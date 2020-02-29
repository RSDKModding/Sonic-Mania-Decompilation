#ifndef OBJ_GHZSETUP_H
#define OBJ_GHZSETUP_H

//Object Class
class GHZSetup : Object {

};

//Entity Class
class EntityGHZSetup : Entity {

};

//Entity Functions
void GHZSetup_Update();
void GHZSetup_EarlyUpdate();
void GHZSetup_LateUpdate();
void GHZSetup_Draw();
void GHZSetup_Setup(void* subtype);
void GHZSetup_StageLoad();
void GHZSetup_GetAttributes();

#endif //!OBJ_GHZSETUP_H
