#ifndef OBJ_MAGPLATFORM_H
#define OBJ_MAGPLATFORM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMagPlatform : Object {

};

// Entity Class
struct EntityMagPlatform : Entity {

};

// Object Struct
extern ObjectMagPlatform *MagPlatform;

// Standard Entity Events
void MagPlatform_Update();
void MagPlatform_LateUpdate();
void MagPlatform_StaticUpdate();
void MagPlatform_Draw();
void MagPlatform_Create(void* data);
void MagPlatform_StageLoad();
void MagPlatform_EditorDraw();
void MagPlatform_EditorLoad();
void MagPlatform_Serialize();

// Extra Entity Functions


#endif //!OBJ_MAGPLATFORM_H
