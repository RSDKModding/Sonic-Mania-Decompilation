#ifndef OBJ_SOL_H
#define OBJ_SOL_H

#include "../../SonicMania.h"

//Object Class
class ObjectSol : public Object {
public:

};

//Entity Class
class EntitySol : public Entity {
public:

};

//Object Entity
ObjectSol Sol;

//Entity Functions
void Sol_Update();
void Sol_EarlyUpdate();
void Sol_LateUpdate();
void Sol_Draw();
void Sol_Setup(void* subtype);
void Sol_StageLoad();
void Sol_GetAttributes();

#endif //!OBJ_SOL_H
