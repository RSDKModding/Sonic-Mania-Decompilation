#ifndef OBJ_VANISHPLATFORM_H
#define OBJ_VANISHPLATFORM_H

#include "../../SonicMania.h"

//Object Class
class ObjectVanishPlatform : public Object {
public:

};

//Entity Class
class EntityVanishPlatform : public Entity {
public:

};

//Object Entity
ObjectVanishPlatform VanishPlatform;

//Entity Functions
void VanishPlatform_Update();
void VanishPlatform_EarlyUpdate();
void VanishPlatform_LateUpdate();
void VanishPlatform_Draw();
void VanishPlatform_Setup(void* subtype);
void VanishPlatform_StageLoad();
void VanishPlatform_GetAttributes();

#endif //!OBJ_VANISHPLATFORM_H
