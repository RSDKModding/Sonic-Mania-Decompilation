#ifndef OBJ_FBZFAN_H
#define OBJ_FBZFAN_H

#include "../../SonicMania.h"

//Object Class
class ObjectFBZFan : public Object {
public:

};

//Entity Class
class EntityFBZFan : public Entity {
public:

};

//Object Entity
ObjectFBZFan FBZFan;

//Entity Functions
void FBZFan_Update();
void FBZFan_EarlyUpdate();
void FBZFan_LateUpdate();
void FBZFan_Draw();
void FBZFan_Setup(void* subtype);
void FBZFan_StageLoad();
void FBZFan_GetAttributes();

#endif //!OBJ_FBZFAN_H
