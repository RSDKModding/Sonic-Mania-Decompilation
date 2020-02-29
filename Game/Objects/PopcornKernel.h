#ifndef OBJ_POPCORNKERNEL_H
#define OBJ_POPCORNKERNEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectPopcornKernel : public Object {
public:

};

//Entity Class
class EntityPopcornKernel : public Entity {
public:

};

//Object Entity
ObjectPopcornKernel PopcornKernel;

//Entity Functions
void PopcornKernel_Update();
void PopcornKernel_EarlyUpdate();
void PopcornKernel_LateUpdate();
void PopcornKernel_Draw();
void PopcornKernel_Setup(void* subtype);
void PopcornKernel_StageLoad();
void PopcornKernel_GetAttributes();

#endif //!OBJ_POPCORNKERNEL_H
