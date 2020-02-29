#ifndef OBJ_PISTON_H
#define OBJ_PISTON_H

#include "../../SonicMania.h"

//Object Class
class ObjectPiston : public Object {
public:

};

//Entity Class
class EntityPiston : public Entity {
public:

};

//Object Entity
ObjectPiston Piston;

//Entity Functions
void Piston_Update();
void Piston_EarlyUpdate();
void Piston_LateUpdate();
void Piston_Draw();
void Piston_Setup(void* subtype);
void Piston_StageLoad();
void Piston_GetAttributes();

#endif //!OBJ_PISTON_H
