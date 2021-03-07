#ifndef OBJ_GASPLATFORM_H
#define OBJ_GASPLATFORM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectGasPlatform : Object {

};

// Entity Class
struct EntityGasPlatform : Entity {

};

// Object Struct
extern ObjectGasPlatform *GasPlatform;

// Standard Entity Events
void GasPlatform_Update();
void GasPlatform_LateUpdate();
void GasPlatform_StaticUpdate();
void GasPlatform_Draw();
void GasPlatform_Create(void* data);
void GasPlatform_StageLoad();
void GasPlatform_EditorDraw();
void GasPlatform_EditorLoad();
void GasPlatform_Serialize();

// Extra Entity Functions


#endif //!OBJ_GASPLATFORM_H
