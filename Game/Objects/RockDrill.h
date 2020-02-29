#ifndef OBJ_ROCKDRILL_H
#define OBJ_ROCKDRILL_H

#include "../../SonicMania.h"

//Object Class
class ObjectRockDrill : public Object {
public:

};

//Entity Class
class EntityRockDrill : public Entity {
public:

};

//Object Entity
ObjectRockDrill RockDrill;

//Entity Functions
void RockDrill_Update();
void RockDrill_EarlyUpdate();
void RockDrill_LateUpdate();
void RockDrill_Draw();
void RockDrill_Setup(void* subtype);
void RockDrill_StageLoad();
void RockDrill_GetAttributes();

#endif //!OBJ_ROCKDRILL_H
