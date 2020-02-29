#ifndef OBJ_AQUIS_H
#define OBJ_AQUIS_H

#include "../../SonicMania.h"

//Object Class
class ObjectAquis : public Object {
public:

};

//Entity Class
class EntityAquis : public Entity {
public:

};

//Object Entity
ObjectAquis Aquis;

//Entity Functions
void Aquis_Update();
void Aquis_EarlyUpdate();
void Aquis_LateUpdate();
void Aquis_Draw();
void Aquis_Setup(void* subtype);
void Aquis_StageLoad();
void Aquis_GetAttributes();

#endif //!OBJ_AQUIS_H
