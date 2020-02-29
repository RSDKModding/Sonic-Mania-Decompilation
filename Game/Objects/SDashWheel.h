#ifndef OBJ_SDASHWHEEL_H
#define OBJ_SDASHWHEEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectSDashWheel : public Object {
public:

};

//Entity Class
class EntitySDashWheel : public Entity {
public:

};

//Object Entity
ObjectSDashWheel SDashWheel;

//Entity Functions
void SDashWheel_Update();
void SDashWheel_EarlyUpdate();
void SDashWheel_LateUpdate();
void SDashWheel_Draw();
void SDashWheel_Setup(void* subtype);
void SDashWheel_StageLoad();
void SDashWheel_GetAttributes();

#endif //!OBJ_SDASHWHEEL_H
