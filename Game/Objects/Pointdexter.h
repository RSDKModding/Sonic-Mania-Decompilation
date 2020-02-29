#ifndef OBJ_POINTDEXTER_H
#define OBJ_POINTDEXTER_H

#include "../../SonicMania.h"

//Object Class
class ObjectPointdexter : public Object {
public:

};

//Entity Class
class EntityPointdexter : public Entity {
public:

};

//Object Entity
ObjectPointdexter Pointdexter;

//Entity Functions
void Pointdexter_Update();
void Pointdexter_EarlyUpdate();
void Pointdexter_LateUpdate();
void Pointdexter_Draw();
void Pointdexter_Setup(void* subtype);
void Pointdexter_StageLoad();
void Pointdexter_GetAttributes();

#endif //!OBJ_POINTDEXTER_H
