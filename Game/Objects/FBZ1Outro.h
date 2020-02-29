#ifndef OBJ_FBZ1OUTRO_H
#define OBJ_FBZ1OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectFBZ1Outro : public Object {
public:

};

//Entity Class
class EntityFBZ1Outro : public Entity {
public:

};

//Object Entity
ObjectFBZ1Outro FBZ1Outro;

//Entity Functions
void FBZ1Outro_Update();
void FBZ1Outro_EarlyUpdate();
void FBZ1Outro_LateUpdate();
void FBZ1Outro_Draw();
void FBZ1Outro_Setup(void* subtype);
void FBZ1Outro_StageLoad();
void FBZ1Outro_GetAttributes();

#endif //!OBJ_FBZ1OUTRO_H
