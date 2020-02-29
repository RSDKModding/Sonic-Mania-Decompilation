#ifndef OBJ_SHIVERSAW_H
#define OBJ_SHIVERSAW_H

#include "../../SonicMania.h"

//Object Class
class ObjectShiversaw : public Object {
public:

};

//Entity Class
class EntityShiversaw : public Entity {
public:

};

//Object Entity
ObjectShiversaw Shiversaw;

//Entity Functions
void Shiversaw_Update();
void Shiversaw_EarlyUpdate();
void Shiversaw_LateUpdate();
void Shiversaw_Draw();
void Shiversaw_Setup(void* subtype);
void Shiversaw_StageLoad();
void Shiversaw_GetAttributes();

#endif //!OBJ_SHIVERSAW_H
