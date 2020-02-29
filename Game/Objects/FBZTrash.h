#ifndef OBJ_FBZTRASH_H
#define OBJ_FBZTRASH_H

//Object Class
class FBZTrash : Object {

};

//Entity Class
class EntityFBZTrash : Entity {

};

//Entity Functions
void FBZTrash_Update();
void FBZTrash_EarlyUpdate();
void FBZTrash_LateUpdate();
void FBZTrash_Draw();
void FBZTrash_Setup(void* subtype);
void FBZTrash_StageLoad();
void FBZTrash_GetAttributes();

#endif //!OBJ_FBZTRASH_H
