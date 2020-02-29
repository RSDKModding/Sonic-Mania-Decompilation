#ifndef OBJ_FBZSTORM_H
#define OBJ_FBZSTORM_H

//Object Class
class FBZStorm : Object {

};

//Entity Class
class EntityFBZStorm : Entity {

};

//Entity Functions
void FBZStorm_Update();
void FBZStorm_EarlyUpdate();
void FBZStorm_LateUpdate();
void FBZStorm_Draw();
void FBZStorm_Setup(void* subtype);
void FBZStorm_StageLoad();
void FBZStorm_GetAttributes();

#endif //!OBJ_FBZSTORM_H
