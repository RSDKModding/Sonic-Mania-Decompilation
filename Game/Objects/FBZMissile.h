#ifndef OBJ_FBZMISSILE_H
#define OBJ_FBZMISSILE_H

#include "../../SonicMania.h"

//Object Class
class ObjectFBZMissile : public Object {
public:

};

//Entity Class
class EntityFBZMissile : public Entity {
public:

};

//Object Entity
ObjectFBZMissile FBZMissile;

//Entity Functions
void FBZMissile_Update();
void FBZMissile_EarlyUpdate();
void FBZMissile_LateUpdate();
void FBZMissile_Draw();
void FBZMissile_Setup(void* subtype);
void FBZMissile_StageLoad();
void FBZMissile_GetAttributes();

#endif //!OBJ_FBZMISSILE_H
