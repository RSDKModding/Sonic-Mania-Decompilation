#ifndef OBJ_ICE_H
#define OBJ_ICE_H

#include "../../SonicMania.h"

//Object Class
class ObjectIce : public Object {
public:

};

//Entity Class
class EntityIce : public Entity {
public:

};

//Object Entity
ObjectIce Ice;

//Entity Functions
void Ice_Update();
void Ice_EarlyUpdate();
void Ice_LateUpdate();
void Ice_Draw();
void Ice_Setup(void* subtype);
void Ice_StageLoad();
void Ice_GetAttributes();

#endif //!OBJ_ICE_H
