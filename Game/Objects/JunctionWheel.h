#ifndef OBJ_JUNCTIONWHEEL_H
#define OBJ_JUNCTIONWHEEL_H

#include "../../SonicMania.h"

//Object Class
class ObjectJunctionWheel : public Object {
public:

};

//Entity Class
class EntityJunctionWheel : public Entity {
public:

};

//Object Entity
ObjectJunctionWheel JunctionWheel;

//Entity Functions
void JunctionWheel_Update();
void JunctionWheel_EarlyUpdate();
void JunctionWheel_LateUpdate();
void JunctionWheel_Draw();
void JunctionWheel_Setup(void* subtype);
void JunctionWheel_StageLoad();
void JunctionWheel_GetAttributes();

#endif //!OBJ_JUNCTIONWHEEL_H
