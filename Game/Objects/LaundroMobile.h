#ifndef OBJ_LAUNDROMOBILE_H
#define OBJ_LAUNDROMOBILE_H

//Object Class
class LaundroMobile : Object {

};

//Entity Class
class EntityLaundroMobile : Entity {

};

//Entity Functions
void LaundroMobile_Update();
void LaundroMobile_EarlyUpdate();
void LaundroMobile_LateUpdate();
void LaundroMobile_Draw();
void LaundroMobile_Setup(void* subtype);
void LaundroMobile_StageLoad();
void LaundroMobile_GetAttributes();

#endif //!OBJ_LAUNDROMOBILE_H
