#ifndef OBJ_RISINGLAVA_H
#define OBJ_RISINGLAVA_H

#include "../../SonicMania.h"

//Object Class
class ObjectRisingLava : public Object {
public:

};

//Entity Class
class EntityRisingLava : public Entity {
public:

};

//Object Entity
ObjectRisingLava RisingLava;

//Entity Functions
void RisingLava_Update();
void RisingLava_EarlyUpdate();
void RisingLava_LateUpdate();
void RisingLava_Draw();
void RisingLava_Setup(void* subtype);
void RisingLava_StageLoad();
void RisingLava_GetAttributes();

#endif //!OBJ_RISINGLAVA_H
