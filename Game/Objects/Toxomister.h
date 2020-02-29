#ifndef OBJ_TOXOMISTER_H
#define OBJ_TOXOMISTER_H

#include "../../SonicMania.h"

//Object Class
class ObjectToxomister : public Object {
public:

};

//Entity Class
class EntityToxomister : public Entity {
public:

};

//Object Entity
ObjectToxomister Toxomister;

//Entity Functions
void Toxomister_Update();
void Toxomister_EarlyUpdate();
void Toxomister_LateUpdate();
void Toxomister_Draw();
void Toxomister_Setup(void* subtype);
void Toxomister_StageLoad();
void Toxomister_GetAttributes();

#endif //!OBJ_TOXOMISTER_H
