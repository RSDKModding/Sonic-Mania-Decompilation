#ifndef OBJ_PSZ2SETUP_H
#define OBJ_PSZ2SETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectPSZ2Setup : public Object {
public:

};

//Entity Class
class EntityPSZ2Setup : public Entity {
public:

};

//Object Entity
ObjectPSZ2Setup PSZ2Setup;

//Entity Functions
void PSZ2Setup_Update();
void PSZ2Setup_EarlyUpdate();
void PSZ2Setup_LateUpdate();
void PSZ2Setup_Draw();
void PSZ2Setup_Setup(void* subtype);
void PSZ2Setup_StageLoad();
void PSZ2Setup_GetAttributes();

#endif //!OBJ_PSZ2SETUP_H
