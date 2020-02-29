#ifndef OBJ_GIGAMETAL_H
#define OBJ_GIGAMETAL_H

#include "../../SonicMania.h"

//Object Class
class ObjectGigaMetal : public Object {
public:

};

//Entity Class
class EntityGigaMetal : public Entity {
public:

};

//Object Entity
ObjectGigaMetal GigaMetal;

//Entity Functions
void GigaMetal_Update();
void GigaMetal_EarlyUpdate();
void GigaMetal_LateUpdate();
void GigaMetal_Draw();
void GigaMetal_Setup(void* subtype);
void GigaMetal_StageLoad();
void GigaMetal_GetAttributes();

#endif //!OBJ_GIGAMETAL_H
