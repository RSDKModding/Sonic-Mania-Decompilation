#ifndef OBJ_FORCESPIN_H
#define OBJ_FORCESPIN_H

#include "../../SonicMania.h"

//Object Class
class ObjectForceSpin : public Object {
public:

};

//Entity Class
class EntityForceSpin : public Entity {
public:

};

//Object Entity
ObjectForceSpin ForceSpin;

//Entity Functions
void ForceSpin_Update();
void ForceSpin_EarlyUpdate();
void ForceSpin_LateUpdate();
void ForceSpin_Draw();
void ForceSpin_Setup(void* subtype);
void ForceSpin_StageLoad();
void ForceSpin_GetAttributes();

#endif //!OBJ_FORCESPIN_H
