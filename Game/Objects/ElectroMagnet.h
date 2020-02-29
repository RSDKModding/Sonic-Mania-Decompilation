#ifndef OBJ_ELECTROMAGNET_H
#define OBJ_ELECTROMAGNET_H

#include "../../SonicMania.h"

//Object Class
class ObjectElectroMagnet : public Object {
public:

};

//Entity Class
class EntityElectroMagnet : public Entity {
public:

};

//Object Entity
ObjectElectroMagnet ElectroMagnet;

//Entity Functions
void ElectroMagnet_Update();
void ElectroMagnet_EarlyUpdate();
void ElectroMagnet_LateUpdate();
void ElectroMagnet_Draw();
void ElectroMagnet_Setup(void* subtype);
void ElectroMagnet_StageLoad();
void ElectroMagnet_GetAttributes();

#endif //!OBJ_ELECTROMAGNET_H
