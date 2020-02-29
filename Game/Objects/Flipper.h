#ifndef OBJ_FLIPPER_H
#define OBJ_FLIPPER_H

#include "../../SonicMania.h"

//Object Class
class ObjectFlipper : public Object {
public:

};

//Entity Class
class EntityFlipper : public Entity {
public:

};

//Object Entity
ObjectFlipper Flipper;

//Entity Functions
void Flipper_Update();
void Flipper_EarlyUpdate();
void Flipper_LateUpdate();
void Flipper_Draw();
void Flipper_Setup(void* subtype);
void Flipper_StageLoad();
void Flipper_GetAttributes();

#endif //!OBJ_FLIPPER_H
