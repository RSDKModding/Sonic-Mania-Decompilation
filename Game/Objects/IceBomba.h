#ifndef OBJ_ICEBOMBA_H
#define OBJ_ICEBOMBA_H

#include "../../SonicMania.h"

//Object Class
class ObjectIceBomba : public Object {
public:

};

//Entity Class
class EntityIceBomba : public Entity {
public:

};

//Object Entity
ObjectIceBomba IceBomba;

//Entity Functions
void IceBomba_Update();
void IceBomba_EarlyUpdate();
void IceBomba_LateUpdate();
void IceBomba_Draw();
void IceBomba_Setup(void* subtype);
void IceBomba_StageLoad();
void IceBomba_GetAttributes();

#endif //!OBJ_ICEBOMBA_H
