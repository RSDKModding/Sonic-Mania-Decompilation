#ifndef OBJ_ERZRIDER_H
#define OBJ_ERZRIDER_H

#include "../../SonicMania.h"

//Object Class
class ObjectERZRider : public Object {
public:

};

//Entity Class
class EntityERZRider : public Entity {
public:

};

//Object Entity
ObjectERZRider ERZRider;

//Entity Functions
void ERZRider_Update();
void ERZRider_EarlyUpdate();
void ERZRider_LateUpdate();
void ERZRider_Draw();
void ERZRider_Setup(void* subtype);
void ERZRider_StageLoad();
void ERZRider_GetAttributes();

#endif //!OBJ_ERZRIDER_H
