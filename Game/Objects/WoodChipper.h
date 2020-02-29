#ifndef OBJ_WOODCHIPPER_H
#define OBJ_WOODCHIPPER_H

#include "../../SonicMania.h"

//Object Class
class ObjectWoodChipper : public Object {
public:

};

//Entity Class
class EntityWoodChipper : public Entity {
public:

};

//Object Entity
ObjectWoodChipper WoodChipper;

//Entity Functions
void WoodChipper_Update();
void WoodChipper_EarlyUpdate();
void WoodChipper_LateUpdate();
void WoodChipper_Draw();
void WoodChipper_Setup(void* subtype);
void WoodChipper_StageLoad();
void WoodChipper_GetAttributes();

#endif //!OBJ_WOODCHIPPER_H
