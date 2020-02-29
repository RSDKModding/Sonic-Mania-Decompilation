#ifndef OBJ_PHANTOMSHINOBI_H
#define OBJ_PHANTOMSHINOBI_H

#include "../../SonicMania.h"

//Object Class
class ObjectPhantomShinobi : public Object {
public:

};

//Entity Class
class EntityPhantomShinobi : public Entity {
public:

};

//Object Entity
ObjectPhantomShinobi PhantomShinobi;

//Entity Functions
void PhantomShinobi_Update();
void PhantomShinobi_EarlyUpdate();
void PhantomShinobi_LateUpdate();
void PhantomShinobi_Draw();
void PhantomShinobi_Setup(void* subtype);
void PhantomShinobi_StageLoad();
void PhantomShinobi_GetAttributes();

#endif //!OBJ_PHANTOMSHINOBI_H
