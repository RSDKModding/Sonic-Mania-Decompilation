#ifndef OBJ_MECHABU_H
#define OBJ_MECHABU_H

#include "../../SonicMania.h"

//Object Class
class ObjectMechaBu : public Object {
public:

};

//Entity Class
class EntityMechaBu : public Entity {
public:

};

//Object Entity
ObjectMechaBu MechaBu;

//Entity Functions
void MechaBu_Update();
void MechaBu_EarlyUpdate();
void MechaBu_LateUpdate();
void MechaBu_Draw();
void MechaBu_Setup(void* subtype);
void MechaBu_StageLoad();
void MechaBu_GetAttributes();

#endif //!OBJ_MECHABU_H
