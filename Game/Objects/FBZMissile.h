#ifndef OBJ_FBZMISSILE_H
#define OBJ_FBZMISSILE_H

//Object Class
class FBZMissile : Object {

};

//Entity Class
class EntityFBZMissile : Entity {

};

//Entity Functions
void FBZMissile_Update();
void FBZMissile_EarlyUpdate();
void FBZMissile_LateUpdate();
void FBZMissile_Draw();
void FBZMissile_Setup(void* subtype);
void FBZMissile_StageLoad();
void FBZMissile_GetAttributes();

#endif //!OBJ_FBZMISSILE_H
