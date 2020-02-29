#ifndef OBJ_BLASTER_H
#define OBJ_BLASTER_H

#include "../../SonicMania.h"

//Object Class
class ObjectBlaster : public Object {
public:

};

//Entity Class
class EntityBlaster : public Entity {
public:

};

//Object Entity
ObjectBlaster Blaster;

//Entity Functions
void Blaster_Update();
void Blaster_EarlyUpdate();
void Blaster_LateUpdate();
void Blaster_Draw();
void Blaster_Setup(void* subtype);
void Blaster_StageLoad();
void Blaster_GetAttributes();

#endif //!OBJ_BLASTER_H
