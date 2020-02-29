#ifndef OBJ_HATTERKILLER_H
#define OBJ_HATTERKILLER_H

#include "../../SonicMania.h"

//Object Class
class ObjectHatterkiller : public Object {
public:

};

//Entity Class
class EntityHatterkiller : public Entity {
public:

};

//Object Entity
ObjectHatterkiller Hatterkiller;

//Entity Functions
void Hatterkiller_Update();
void Hatterkiller_EarlyUpdate();
void Hatterkiller_LateUpdate();
void Hatterkiller_Draw();
void Hatterkiller_Setup(void* subtype);
void Hatterkiller_StageLoad();
void Hatterkiller_GetAttributes();

#endif //!OBJ_HATTERKILLER_H
