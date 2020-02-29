#ifndef OBJ_MAGPLATFORM_H
#define OBJ_MAGPLATFORM_H

#include "../../SonicMania.h"

//Object Class
class ObjectMagPlatform : public Object {
public:

};

//Entity Class
class EntityMagPlatform : public Entity {
public:

};

//Object Entity
ObjectMagPlatform MagPlatform;

//Entity Functions
void MagPlatform_Update();
void MagPlatform_EarlyUpdate();
void MagPlatform_LateUpdate();
void MagPlatform_Draw();
void MagPlatform_Setup(void* subtype);
void MagPlatform_StageLoad();
void MagPlatform_GetAttributes();

#endif //!OBJ_MAGPLATFORM_H
