#ifndef OBJ_CRANE_H
#define OBJ_CRANE_H

#include "../../SonicMania.h"

//Object Class
class ObjectCrane : public Object {
public:

};

//Entity Class
class EntityCrane : public Entity {
public:

};

//Object Entity
ObjectCrane Crane;

//Entity Functions
void Crane_Update();
void Crane_EarlyUpdate();
void Crane_LateUpdate();
void Crane_Draw();
void Crane_Setup(void* subtype);
void Crane_StageLoad();
void Crane_GetAttributes();

#endif //!OBJ_CRANE_H
