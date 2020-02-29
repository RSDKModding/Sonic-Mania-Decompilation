#ifndef OBJ_WOODCHIPPER_H
#define OBJ_WOODCHIPPER_H

//Object Class
class WoodChipper : Object {

};

//Entity Class
class EntityWoodChipper : Entity {

};

//Entity Functions
void WoodChipper_Update();
void WoodChipper_EarlyUpdate();
void WoodChipper_LateUpdate();
void WoodChipper_Draw();
void WoodChipper_Setup(void* subtype);
void WoodChipper_StageLoad();
void WoodChipper_GetAttributes();

#endif //!OBJ_WOODCHIPPER_H
