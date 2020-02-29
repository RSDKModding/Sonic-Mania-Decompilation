#ifndef OBJ_FBZTRASH_H
#define OBJ_FBZTRASH_H

#include "../../SonicMania.h"

//Object Class
class ObjectFBZTrash : public Object {
public:

};

//Entity Class
class EntityFBZTrash : public Entity {
public:

};

//Object Entity
ObjectFBZTrash FBZTrash;

//Entity Functions
void FBZTrash_Update();
void FBZTrash_EarlyUpdate();
void FBZTrash_LateUpdate();
void FBZTrash_Draw();
void FBZTrash_Setup(void* subtype);
void FBZTrash_StageLoad();
void FBZTrash_GetAttributes();

#endif //!OBJ_FBZTRASH_H
