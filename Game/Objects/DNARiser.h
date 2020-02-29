#ifndef OBJ_DNARISER_H
#define OBJ_DNARISER_H

#include "../../SonicMania.h"

//Object Class
class ObjectDNARiser : public Object {
public:

};

//Entity Class
class EntityDNARiser : public Entity {
public:

};

//Object Entity
ObjectDNARiser DNARiser;

//Entity Functions
void DNARiser_Update();
void DNARiser_EarlyUpdate();
void DNARiser_LateUpdate();
void DNARiser_Draw();
void DNARiser_Setup(void* subtype);
void DNARiser_StageLoad();
void DNARiser_GetAttributes();

#endif //!OBJ_DNARISER_H
