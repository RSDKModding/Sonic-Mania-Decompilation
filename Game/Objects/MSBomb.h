#ifndef OBJ_MSBOMB_H
#define OBJ_MSBOMB_H

#include "../../SonicMania.h"

//Object Class
class ObjectMSBomb : public Object {
public:

};

//Entity Class
class EntityMSBomb : public Entity {
public:

};

//Object Entity
ObjectMSBomb MSBomb;

//Entity Functions
void MSBomb_Update();
void MSBomb_EarlyUpdate();
void MSBomb_LateUpdate();
void MSBomb_Draw();
void MSBomb_Setup(void* subtype);
void MSBomb_StageLoad();
void MSBomb_GetAttributes();

#endif //!OBJ_MSBOMB_H
