#ifndef OBJ_GHZSETUP_H
#define OBJ_GHZSETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectGHZSetup : public Object {
public:

};

//Entity Class
class EntityGHZSetup : public Entity {
public:

};

//Object Entity
ObjectGHZSetup GHZSetup;

//Entity Functions
void GHZSetup_Update();
void GHZSetup_EarlyUpdate();
void GHZSetup_LateUpdate();
void GHZSetup_Draw();
void GHZSetup_Setup(void* subtype);
void GHZSetup_StageLoad();
void GHZSetup_GetAttributes();

#endif //!OBJ_GHZSETUP_H
