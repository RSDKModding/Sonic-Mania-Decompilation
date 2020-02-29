#ifndef OBJ_PATHINVERTER_H
#define OBJ_PATHINVERTER_H

#include "../../SonicMania.h"

//Object Class
class ObjectPathInverter : public Object {
public:

};

//Entity Class
class EntityPathInverter : public Entity {
public:

};

//Object Entity
ObjectPathInverter PathInverter;

//Entity Functions
void PathInverter_Update();
void PathInverter_EarlyUpdate();
void PathInverter_LateUpdate();
void PathInverter_Draw();
void PathInverter_Setup(void* subtype);
void PathInverter_StageLoad();
void PathInverter_GetAttributes();

#endif //!OBJ_PATHINVERTER_H
