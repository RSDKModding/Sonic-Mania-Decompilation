#ifndef OBJ_FBZSINKTRASH_H
#define OBJ_FBZSINKTRASH_H

//Object Class
class FBZSinkTrash : Object {

};

//Entity Class
class EntityFBZSinkTrash : Entity {

};

//Entity Functions
void FBZSinkTrash_Update();
void FBZSinkTrash_EarlyUpdate();
void FBZSinkTrash_LateUpdate();
void FBZSinkTrash_Draw();
void FBZSinkTrash_Setup(void* subtype);
void FBZSinkTrash_StageLoad();
void FBZSinkTrash_GetAttributes();

#endif //!OBJ_FBZSINKTRASH_H
