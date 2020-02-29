#ifndef OBJ_MEGACHOPPER_H
#define OBJ_MEGACHOPPER_H

#include "../../SonicMania.h"

//Object Class
class ObjectMegaChopper : public Object {
public:

};

//Entity Class
class EntityMegaChopper : public Entity {
public:

};

//Object Entity
ObjectMegaChopper MegaChopper;

//Entity Functions
void MegaChopper_Update();
void MegaChopper_EarlyUpdate();
void MegaChopper_LateUpdate();
void MegaChopper_Draw();
void MegaChopper_Setup(void* subtype);
void MegaChopper_StageLoad();
void MegaChopper_GetAttributes();

#endif //!OBJ_MEGACHOPPER_H
