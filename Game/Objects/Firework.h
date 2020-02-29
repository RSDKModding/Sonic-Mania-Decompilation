#ifndef OBJ_FIREWORK_H
#define OBJ_FIREWORK_H

#include "../../SonicMania.h"

//Object Class
class ObjectFirework : public Object {
public:

};

//Entity Class
class EntityFirework : public Entity {
public:

};

//Object Entity
ObjectFirework Firework;

//Entity Functions
void Firework_Update();
void Firework_EarlyUpdate();
void Firework_LateUpdate();
void Firework_Draw();
void Firework_Setup(void* subtype);
void Firework_StageLoad();
void Firework_GetAttributes();

#endif //!OBJ_FIREWORK_H
