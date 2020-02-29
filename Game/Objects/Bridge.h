#ifndef OBJ_BRIDGE_H
#define OBJ_BRIDGE_H

#include "../../SonicMania.h"

//Object Class
class ObjectBridge : public Object {
public:

};

//Entity Class
class EntityBridge : public Entity {
public:

};

//Object Entity
ObjectBridge Bridge;

//Entity Functions
void Bridge_Update();
void Bridge_EarlyUpdate();
void Bridge_LateUpdate();
void Bridge_Draw();
void Bridge_Setup(void* subtype);
void Bridge_StageLoad();
void Bridge_GetAttributes();

#endif //!OBJ_BRIDGE_H
