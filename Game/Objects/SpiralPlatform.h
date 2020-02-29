#ifndef OBJ_SPIRALPLATFORM_H
#define OBJ_SPIRALPLATFORM_H

#include "../../SonicMania.h"

//Object Class
class ObjectSpiralPlatform : public Object {
public:

};

//Entity Class
class EntitySpiralPlatform : public Entity {
public:

};

//Object Entity
ObjectSpiralPlatform SpiralPlatform;

//Entity Functions
void SpiralPlatform_Update();
void SpiralPlatform_EarlyUpdate();
void SpiralPlatform_LateUpdate();
void SpiralPlatform_Draw();
void SpiralPlatform_Setup(void* subtype);
void SpiralPlatform_StageLoad();
void SpiralPlatform_GetAttributes();

#endif //!OBJ_SPIRALPLATFORM_H
