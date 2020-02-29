#ifndef OBJ_MEGACHOPPER_H
#define OBJ_MEGACHOPPER_H

//Object Class
class MegaChopper : Object {

};

//Entity Class
class EntityMegaChopper : Entity {

};

//Entity Functions
void MegaChopper_Update();
void MegaChopper_EarlyUpdate();
void MegaChopper_LateUpdate();
void MegaChopper_Draw();
void MegaChopper_Setup(void* subtype);
void MegaChopper_StageLoad();
void MegaChopper_GetAttributes();

#endif //!OBJ_MEGACHOPPER_H
