#ifndef OBJ_SELTZERWATER_H
#define OBJ_SELTZERWATER_H

#include "../../SonicMania.h"

//Object Class
class ObjectSeltzerWater : public Object {
public:

};

//Entity Class
class EntitySeltzerWater : public Entity {
public:

};

//Object Entity
ObjectSeltzerWater SeltzerWater;

//Entity Functions
void SeltzerWater_Update();
void SeltzerWater_EarlyUpdate();
void SeltzerWater_LateUpdate();
void SeltzerWater_Draw();
void SeltzerWater_Setup(void* subtype);
void SeltzerWater_StageLoad();
void SeltzerWater_GetAttributes();

#endif //!OBJ_SELTZERWATER_H
