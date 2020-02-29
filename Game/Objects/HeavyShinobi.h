#ifndef OBJ_HEAVYSHINOBI_H
#define OBJ_HEAVYSHINOBI_H

#include "../../SonicMania.h"

//Object Class
class ObjectHeavyShinobi : public Object {
public:

};

//Entity Class
class EntityHeavyShinobi : public Entity {
public:

};

//Object Entity
ObjectHeavyShinobi HeavyShinobi;

//Entity Functions
void HeavyShinobi_Update();
void HeavyShinobi_EarlyUpdate();
void HeavyShinobi_LateUpdate();
void HeavyShinobi_Draw();
void HeavyShinobi_Setup(void* subtype);
void HeavyShinobi_StageLoad();
void HeavyShinobi_GetAttributes();

#endif //!OBJ_HEAVYSHINOBI_H
