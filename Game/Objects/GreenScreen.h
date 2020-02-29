#ifndef OBJ_GREENSCREEN_H
#define OBJ_GREENSCREEN_H

#include "../../SonicMania.h"

//Object Class
class ObjectGreenScreen : public Object {
public:

};

//Entity Class
class EntityGreenScreen : public Entity {
public:

};

//Object Entity
ObjectGreenScreen GreenScreen;

//Entity Functions
void GreenScreen_Update();
void GreenScreen_EarlyUpdate();
void GreenScreen_LateUpdate();
void GreenScreen_Draw();
void GreenScreen_Setup(void* subtype);
void GreenScreen_StageLoad();
void GreenScreen_GetAttributes();

#endif //!OBJ_GREENSCREEN_H
