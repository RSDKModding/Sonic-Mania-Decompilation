#ifndef OBJ_FBZSETUP_H
#define OBJ_FBZSETUP_H

//Object Class
class FBZSetup : Object {

};

//Entity Class
class EntityFBZSetup : Entity {

};

//Entity Functions
void FBZSetup_Update();
void FBZSetup_EarlyUpdate();
void FBZSetup_LateUpdate();
void FBZSetup_Draw();
void FBZSetup_Setup(void* subtype);
void FBZSetup_StageLoad();
void FBZSetup_GetAttributes();

#endif //!OBJ_FBZSETUP_H
