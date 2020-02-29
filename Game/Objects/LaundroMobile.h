#ifndef OBJ_LAUNDROMOBILE_H
#define OBJ_LAUNDROMOBILE_H

#include "../../SonicMania.h"

//Object Class
class ObjectLaundroMobile : public Object {
public:

};

//Entity Class
class EntityLaundroMobile : public Entity {
public:

};

//Object Entity
ObjectLaundroMobile LaundroMobile;

//Entity Functions
void LaundroMobile_Update();
void LaundroMobile_EarlyUpdate();
void LaundroMobile_LateUpdate();
void LaundroMobile_Draw();
void LaundroMobile_Setup(void* subtype);
void LaundroMobile_StageLoad();
void LaundroMobile_GetAttributes();

#endif //!OBJ_LAUNDROMOBILE_H
