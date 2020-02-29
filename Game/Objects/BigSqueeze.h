#ifndef OBJ_BIGSQUEEZE_H
#define OBJ_BIGSQUEEZE_H

#include "../../SonicMania.h"

//Object Class
class ObjectBigSqueeze : public Object {
public:

};

//Entity Class
class EntityBigSqueeze : public Entity {
public:

};

//Object Entity
ObjectBigSqueeze BigSqueeze;

//Entity Functions
void BigSqueeze_Update();
void BigSqueeze_EarlyUpdate();
void BigSqueeze_LateUpdate();
void BigSqueeze_Draw();
void BigSqueeze_Setup(void* subtype);
void BigSqueeze_StageLoad();
void BigSqueeze_GetAttributes();

#endif //!OBJ_BIGSQUEEZE_H
