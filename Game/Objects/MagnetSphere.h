#ifndef OBJ_MAGNETSPHERE_H
#define OBJ_MAGNETSPHERE_H

#include "../../SonicMania.h"

//Object Class
class ObjectMagnetSphere : public Object {
public:

};

//Entity Class
class EntityMagnetSphere : public Entity {
public:

};

//Object Entity
ObjectMagnetSphere MagnetSphere;

//Entity Functions
void MagnetSphere_Update();
void MagnetSphere_EarlyUpdate();
void MagnetSphere_LateUpdate();
void MagnetSphere_Draw();
void MagnetSphere_Setup(void* subtype);
void MagnetSphere_StageLoad();
void MagnetSphere_GetAttributes();

#endif //!OBJ_MAGNETSPHERE_H
