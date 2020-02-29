#ifndef OBJ_BLADEPOLE_H
#define OBJ_BLADEPOLE_H

#include "../../SonicMania.h"

//Object Class
class ObjectBladePole : public Object {
public:

};

//Entity Class
class EntityBladePole : public Entity {
public:

};

//Object Entity
ObjectBladePole BladePole;

//Entity Functions
void BladePole_Update();
void BladePole_EarlyUpdate();
void BladePole_LateUpdate();
void BladePole_Draw();
void BladePole_Setup(void* subtype);
void BladePole_StageLoad();
void BladePole_GetAttributes();

#endif //!OBJ_BLADEPOLE_H
