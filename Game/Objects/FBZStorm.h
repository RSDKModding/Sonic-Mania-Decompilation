#ifndef OBJ_FBZSTORM_H
#define OBJ_FBZSTORM_H

#include "../../SonicMania.h"

//Object Class
class ObjectFBZStorm : public Object {
public:

};

//Entity Class
class EntityFBZStorm : public Entity {
public:

};

//Object Entity
ObjectFBZStorm FBZStorm;

//Entity Functions
void FBZStorm_Update();
void FBZStorm_EarlyUpdate();
void FBZStorm_LateUpdate();
void FBZStorm_Draw();
void FBZStorm_Setup(void* subtype);
void FBZStorm_StageLoad();
void FBZStorm_GetAttributes();

#endif //!OBJ_FBZSTORM_H
