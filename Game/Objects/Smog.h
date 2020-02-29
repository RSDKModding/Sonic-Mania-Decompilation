#ifndef OBJ_SMOG_H
#define OBJ_SMOG_H

#include "../../SonicMania.h"

//Object Class
class ObjectSmog : public Object {
public:

};

//Entity Class
class EntitySmog : public Entity {
public:

};

//Object Entity
ObjectSmog Smog;

//Entity Functions
void Smog_Update();
void Smog_EarlyUpdate();
void Smog_LateUpdate();
void Smog_Draw();
void Smog_Setup(void* subtype);
void Smog_StageLoad();
void Smog_GetAttributes();

#endif //!OBJ_SMOG_H
