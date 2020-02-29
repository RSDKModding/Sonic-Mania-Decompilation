#ifndef OBJ_ERZSHINOBI_H
#define OBJ_ERZSHINOBI_H

#include "../../SonicMania.h"

//Object Class
class ObjectERZShinobi : public Object {
public:

};

//Entity Class
class EntityERZShinobi : public Entity {
public:

};

//Object Entity
ObjectERZShinobi ERZShinobi;

//Entity Functions
void ERZShinobi_Update();
void ERZShinobi_EarlyUpdate();
void ERZShinobi_LateUpdate();
void ERZShinobi_Draw();
void ERZShinobi_Setup(void* subtype);
void ERZShinobi_StageLoad();
void ERZShinobi_GetAttributes();

#endif //!OBJ_ERZSHINOBI_H
