#ifndef OBJ_SILVERSONIC_H
#define OBJ_SILVERSONIC_H

#include "../../SonicMania.h"

//Object Class
class ObjectSilverSonic : public Object {
public:

};

//Entity Class
class EntitySilverSonic : public Entity {
public:

};

//Object Entity
ObjectSilverSonic SilverSonic;

//Entity Functions
void SilverSonic_Update();
void SilverSonic_EarlyUpdate();
void SilverSonic_LateUpdate();
void SilverSonic_Draw();
void SilverSonic_Setup(void* subtype);
void SilverSonic_StageLoad();
void SilverSonic_GetAttributes();

#endif //!OBJ_SILVERSONIC_H
