#ifndef OBJ_JACOBSLADDER_H
#define OBJ_JACOBSLADDER_H

#include "../../SonicMania.h"

//Object Class
class ObjectJacobsLadder : public Object {
public:

};

//Entity Class
class EntityJacobsLadder : public Entity {
public:

};

//Object Entity
ObjectJacobsLadder JacobsLadder;

//Entity Functions
void JacobsLadder_Update();
void JacobsLadder_EarlyUpdate();
void JacobsLadder_LateUpdate();
void JacobsLadder_Draw();
void JacobsLadder_Setup(void* subtype);
void JacobsLadder_StageLoad();
void JacobsLadder_GetAttributes();

#endif //!OBJ_JACOBSLADDER_H
