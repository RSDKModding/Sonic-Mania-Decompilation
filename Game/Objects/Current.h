#ifndef OBJ_CURRENT_H
#define OBJ_CURRENT_H

#include "../../SonicMania.h"

//Object Class
class ObjectCurrent : public Object {
public:

};

//Entity Class
class EntityCurrent : public Entity {
public:

};

//Object Entity
ObjectCurrent Current;

//Entity Functions
void Current_Update();
void Current_EarlyUpdate();
void Current_LateUpdate();
void Current_Draw();
void Current_Setup(void* subtype);
void Current_StageLoad();
void Current_GetAttributes();

#endif //!OBJ_CURRENT_H
