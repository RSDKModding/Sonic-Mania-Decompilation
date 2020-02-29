#ifndef OBJ_SP500MKII_H
#define OBJ_SP500MKII_H

#include "../../SonicMania.h"

//Object Class
class ObjectSP500MkII : public Object {
public:

};

//Entity Class
class EntitySP500MkII : public Entity {
public:

};

//Object Entity
ObjectSP500MkII SP500MkII;

//Entity Functions
void SP500MkII_Update();
void SP500MkII_EarlyUpdate();
void SP500MkII_LateUpdate();
void SP500MkII_Draw();
void SP500MkII_Setup(void* subtype);
void SP500MkII_StageLoad();
void SP500MkII_GetAttributes();

#endif //!OBJ_SP500MKII_H
