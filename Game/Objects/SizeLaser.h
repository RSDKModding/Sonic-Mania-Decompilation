#ifndef OBJ_SIZELASER_H
#define OBJ_SIZELASER_H

#include "../../SonicMania.h"

//Object Class
class ObjectSizeLaser : public Object {
public:

};

//Entity Class
class EntitySizeLaser : public Entity {
public:

};

//Object Entity
ObjectSizeLaser SizeLaser;

//Entity Functions
void SizeLaser_Update();
void SizeLaser_EarlyUpdate();
void SizeLaser_LateUpdate();
void SizeLaser_Draw();
void SizeLaser_Setup(void* subtype);
void SizeLaser_StageLoad();
void SizeLaser_GetAttributes();

#endif //!OBJ_SIZELASER_H
