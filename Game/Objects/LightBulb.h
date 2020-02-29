#ifndef OBJ_LIGHTBULB_H
#define OBJ_LIGHTBULB_H

#include "../../SonicMania.h"

//Object Class
class ObjectLightBulb : public Object {
public:

};

//Entity Class
class EntityLightBulb : public Entity {
public:

};

//Object Entity
ObjectLightBulb LightBulb;

//Entity Functions
void LightBulb_Update();
void LightBulb_EarlyUpdate();
void LightBulb_LateUpdate();
void LightBulb_Draw();
void LightBulb_Setup(void* subtype);
void LightBulb_StageLoad();
void LightBulb_GetAttributes();

#endif //!OBJ_LIGHTBULB_H
