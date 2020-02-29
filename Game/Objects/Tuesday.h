#ifndef OBJ_TUESDAY_H
#define OBJ_TUESDAY_H

#include "../../SonicMania.h"

//Object Class
class ObjectTuesday : public Object {
public:

};

//Entity Class
class EntityTuesday : public Entity {
public:

};

//Object Entity
ObjectTuesday Tuesday;

//Entity Functions
void Tuesday_Update();
void Tuesday_EarlyUpdate();
void Tuesday_LateUpdate();
void Tuesday_Draw();
void Tuesday_Setup(void* subtype);
void Tuesday_StageLoad();
void Tuesday_GetAttributes();

#endif //!OBJ_TUESDAY_H
