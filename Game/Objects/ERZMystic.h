#ifndef OBJ_ERZMYSTIC_H
#define OBJ_ERZMYSTIC_H

#include "../../SonicMania.h"

//Object Class
class ObjectERZMystic : public Object {
public:

};

//Entity Class
class EntityERZMystic : public Entity {
public:

};

//Object Entity
ObjectERZMystic ERZMystic;

//Entity Functions
void ERZMystic_Update();
void ERZMystic_EarlyUpdate();
void ERZMystic_LateUpdate();
void ERZMystic_Draw();
void ERZMystic_Setup(void* subtype);
void ERZMystic_StageLoad();
void ERZMystic_GetAttributes();

#endif //!OBJ_ERZMYSTIC_H
