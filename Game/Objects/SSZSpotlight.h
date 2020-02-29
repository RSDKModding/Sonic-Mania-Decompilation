#ifndef OBJ_SSZSPOTLIGHT_H
#define OBJ_SSZSPOTLIGHT_H

#include "../../SonicMania.h"

//Object Class
class ObjectSSZSpotlight : public Object {
public:

};

//Entity Class
class EntitySSZSpotlight : public Entity {
public:

};

//Object Entity
ObjectSSZSpotlight SSZSpotlight;

//Entity Functions
void SSZSpotlight_Update();
void SSZSpotlight_EarlyUpdate();
void SSZSpotlight_LateUpdate();
void SSZSpotlight_Draw();
void SSZSpotlight_Setup(void* subtype);
void SSZSpotlight_StageLoad();
void SSZSpotlight_GetAttributes();

#endif //!OBJ_SSZSPOTLIGHT_H
