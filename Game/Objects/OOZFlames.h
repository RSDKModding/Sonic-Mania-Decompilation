#ifndef OBJ_OOZFLAMES_H
#define OBJ_OOZFLAMES_H

#include "../../SonicMania.h"

//Object Class
class ObjectOOZFlames : public Object {
public:

};

//Entity Class
class EntityOOZFlames : public Entity {
public:

};

//Object Entity
ObjectOOZFlames OOZFlames;

//Entity Functions
void OOZFlames_Update();
void OOZFlames_EarlyUpdate();
void OOZFlames_LateUpdate();
void OOZFlames_Draw();
void OOZFlames_Setup(void* subtype);
void OOZFlames_StageLoad();
void OOZFlames_GetAttributes();

#endif //!OBJ_OOZFLAMES_H
