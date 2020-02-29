#ifndef OBJ_MOTOBUG_H
#define OBJ_MOTOBUG_H

#include "../../SonicMania.h"

//Object Class
class ObjectMotobug : public Object {
public:

};

//Entity Class
class EntityMotobug : public Entity {
public:

};

//Object Entity
ObjectMotobug Motobug;

//Entity Functions
void Motobug_Update();
void Motobug_EarlyUpdate();
void Motobug_LateUpdate();
void Motobug_Draw();
void Motobug_Setup(void* subtype);
void Motobug_StageLoad();
void Motobug_GetAttributes();

#endif //!OBJ_MOTOBUG_H
