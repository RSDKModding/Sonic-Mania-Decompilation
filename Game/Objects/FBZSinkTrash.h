#ifndef OBJ_FBZSINKTRASH_H
#define OBJ_FBZSINKTRASH_H

#include "../../SonicMania.h"

//Object Class
class ObjectFBZSinkTrash : public Object {
public:

};

//Entity Class
class EntityFBZSinkTrash : public Entity {
public:

};

//Object Entity
ObjectFBZSinkTrash FBZSinkTrash;

//Entity Functions
void FBZSinkTrash_Update();
void FBZSinkTrash_EarlyUpdate();
void FBZSinkTrash_LateUpdate();
void FBZSinkTrash_Draw();
void FBZSinkTrash_Setup(void* subtype);
void FBZSinkTrash_StageLoad();
void FBZSinkTrash_GetAttributes();

#endif //!OBJ_FBZSINKTRASH_H
