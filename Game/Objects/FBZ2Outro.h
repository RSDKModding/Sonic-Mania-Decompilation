#ifndef OBJ_FBZ2OUTRO_H
#define OBJ_FBZ2OUTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectFBZ2Outro : public Object {
public:

};

//Entity Class
class EntityFBZ2Outro : public Entity {
public:

};

//Object Entity
ObjectFBZ2Outro FBZ2Outro;

//Entity Functions
void FBZ2Outro_Update();
void FBZ2Outro_EarlyUpdate();
void FBZ2Outro_LateUpdate();
void FBZ2Outro_Draw();
void FBZ2Outro_Setup(void* subtype);
void FBZ2Outro_StageLoad();
void FBZ2Outro_GetAttributes();

#endif //!OBJ_FBZ2OUTRO_H
