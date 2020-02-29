#ifndef OBJ_PROJECTILE_H
#define OBJ_PROJECTILE_H

#include "../../SonicMania.h"

//Object Class
class ObjectProjectile : public Object {
public:

};

//Entity Class
class EntityProjectile : public Entity {
public:

};

//Object Entity
ObjectProjectile Projectile;

//Entity Functions
void Projectile_Update();
void Projectile_EarlyUpdate();
void Projectile_LateUpdate();
void Projectile_Draw();
void Projectile_Setup(void* subtype);
void Projectile_StageLoad();
void Projectile_GetAttributes();

#endif //!OBJ_PROJECTILE_H
