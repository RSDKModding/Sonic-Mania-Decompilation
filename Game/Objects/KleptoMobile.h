#ifndef OBJ_KLEPTOMOBILE_H
#define OBJ_KLEPTOMOBILE_H

#include "../../SonicMania.h"

//Object Class
class ObjectKleptoMobile : public Object {
public:

};

//Entity Class
class EntityKleptoMobile : public Entity {
public:

};

//Object Entity
ObjectKleptoMobile KleptoMobile;

//Entity Functions
void KleptoMobile_Update();
void KleptoMobile_EarlyUpdate();
void KleptoMobile_LateUpdate();
void KleptoMobile_Draw();
void KleptoMobile_Setup(void* subtype);
void KleptoMobile_StageLoad();
void KleptoMobile_GetAttributes();

#endif //!OBJ_KLEPTOMOBILE_H
