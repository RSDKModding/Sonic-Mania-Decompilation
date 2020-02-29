#ifndef OBJ_SPRINGBOARD_H
#define OBJ_SPRINGBOARD_H

#include "../../SonicMania.h"

//Object Class
class ObjectSpringboard : public Object {
public:

};

//Entity Class
class EntitySpringboard : public Entity {
public:

};

//Object Entity
ObjectSpringboard Springboard;

//Entity Functions
void Springboard_Update();
void Springboard_EarlyUpdate();
void Springboard_LateUpdate();
void Springboard_Draw();
void Springboard_Setup(void* subtype);
void Springboard_StageLoad();
void Springboard_GetAttributes();

#endif //!OBJ_SPRINGBOARD_H
