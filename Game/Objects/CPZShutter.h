#ifndef OBJ_CPZSHUTTER_H
#define OBJ_CPZSHUTTER_H

#include "../../SonicMania.h"

//Object Class
class ObjectCPZShutter : public Object {
public:

};

//Entity Class
class EntityCPZShutter : public Entity {
public:

};

//Object Entity
ObjectCPZShutter CPZShutter;

//Entity Functions
void CPZShutter_Update();
void CPZShutter_EarlyUpdate();
void CPZShutter_LateUpdate();
void CPZShutter_Draw();
void CPZShutter_Setup(void* subtype);
void CPZShutter_StageLoad();
void CPZShutter_GetAttributes();

#endif //!OBJ_CPZSHUTTER_H
