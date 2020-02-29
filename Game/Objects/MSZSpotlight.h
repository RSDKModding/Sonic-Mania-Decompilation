#ifndef OBJ_MSZSPOTLIGHT_H
#define OBJ_MSZSPOTLIGHT_H

#include "../../SonicMania.h"

//Object Class
class ObjectMSZSpotlight : public Object {
public:

};

//Entity Class
class EntityMSZSpotlight : public Entity {
public:

};

//Object Entity
ObjectMSZSpotlight MSZSpotlight;

//Entity Functions
void MSZSpotlight_Update();
void MSZSpotlight_EarlyUpdate();
void MSZSpotlight_LateUpdate();
void MSZSpotlight_Draw();
void MSZSpotlight_Setup(void* subtype);
void MSZSpotlight_StageLoad();
void MSZSpotlight_GetAttributes();

#endif //!OBJ_MSZSPOTLIGHT_H
