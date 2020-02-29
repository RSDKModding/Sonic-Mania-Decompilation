#ifndef OBJ_SSZ2SETUP_H
#define OBJ_SSZ2SETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectSSZ2Setup : public Object {
public:

};

//Entity Class
class EntitySSZ2Setup : public Entity {
public:

};

//Object Entity
ObjectSSZ2Setup SSZ2Setup;

//Entity Functions
void SSZ2Setup_Update();
void SSZ2Setup_EarlyUpdate();
void SSZ2Setup_LateUpdate();
void SSZ2Setup_Draw();
void SSZ2Setup_Setup(void* subtype);
void SSZ2Setup_StageLoad();
void SSZ2Setup_GetAttributes();

#endif //!OBJ_SSZ2SETUP_H
