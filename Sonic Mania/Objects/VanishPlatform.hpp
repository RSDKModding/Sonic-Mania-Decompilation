#ifndef OBJ_VANISHPLATFORM_H
#define OBJ_VANISHPLATFORM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectVanishPlatform : Object {

};

// Entity Class
struct EntityVanishPlatform : Entity {

};

// Object Entity
extern ObjectVanishPlatform VanishPlatform;

// Standard Entity Events
void VanishPlatform_Update();
void VanishPlatform_LateUpdate();
void VanishPlatform_StaticUpdate();
void VanishPlatform_Draw();
void VanishPlatform_Create(void* data);
void VanishPlatform_StageLoad();
void VanishPlatform_EditorDraw();
void VanishPlatform_EditorLoad();
void VanishPlatform_Serialize();

// Extra Entity Functions


#endif //!OBJ_VANISHPLATFORM_H
