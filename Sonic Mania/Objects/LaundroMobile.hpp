#ifndef OBJ_LAUNDROMOBILE_H
#define OBJ_LAUNDROMOBILE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLaundroMobile : Object{

};

// Entity Class
struct EntityLaundroMobile : Entity {

};

// Object Struct
extern ObjectLaundroMobile *LaundroMobile;

// Standard Entity Events
void LaundroMobile_Update();
void LaundroMobile_LateUpdate();
void LaundroMobile_StaticUpdate();
void LaundroMobile_Draw();
void LaundroMobile_Create(void* data);
void LaundroMobile_StageLoad();
void LaundroMobile_EditorDraw();
void LaundroMobile_EditorLoad();
void LaundroMobile_Serialize();

// Extra Entity Functions


#endif //!OBJ_LAUNDROMOBILE_H
