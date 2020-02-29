#ifndef OBJ_ERZSETUP_H
#define OBJ_ERZSETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectERZSetup : public Object {
public:

};

//Entity Class
class EntityERZSetup : public Entity {
public:

};

//Object Entity
ObjectERZSetup ERZSetup;

//Entity Functions
void ERZSetup_Update();
void ERZSetup_EarlyUpdate();
void ERZSetup_LateUpdate();
void ERZSetup_Draw();
void ERZSetup_Setup(void* subtype);
void ERZSetup_StageLoad();
void ERZSetup_GetAttributes();

#endif //!OBJ_ERZSETUP_H
