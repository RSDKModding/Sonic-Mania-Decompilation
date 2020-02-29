#ifndef OBJ_FBZSETUP_H
#define OBJ_FBZSETUP_H

#include "../../SonicMania.h"

//Object Class
class ObjectFBZSetup : public Object {
public:

};

//Entity Class
class EntityFBZSetup : public Entity {
public:

};

//Object Entity
ObjectFBZSetup FBZSetup;

//Entity Functions
void FBZSetup_Update();
void FBZSetup_EarlyUpdate();
void FBZSetup_LateUpdate();
void FBZSetup_Draw();
void FBZSetup_Setup(void* subtype);
void FBZSetup_StageLoad();
void FBZSetup_GetAttributes();

#endif //!OBJ_FBZSETUP_H
