#ifndef OBJ_ZONE_H
#define OBJ_ZONE_H

#include "../../SonicMania.h"

//Object Class
class ObjectZone : public Object {
public:

};

//Entity Class
class EntityZone : public Entity {
public:

};

//Object Entity
ObjectZone Zone;

//Entity Functions
void Zone_Update();
void Zone_EarlyUpdate();
void Zone_LateUpdate();
void Zone_Draw();
void Zone_Setup(void* subtype);
void Zone_StageLoad();
void Zone_GetAttributes();

#endif //!OBJ_ZONE_H
