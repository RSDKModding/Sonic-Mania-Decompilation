#ifndef OBJ_CPZ1INTRO_H
#define OBJ_CPZ1INTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectCPZ1Intro : public Object {
public:

};

//Entity Class
class EntityCPZ1Intro : public Entity {
public:

};

//Object Entity
ObjectCPZ1Intro CPZ1Intro;

//Entity Functions
void CPZ1Intro_Update();
void CPZ1Intro_EarlyUpdate();
void CPZ1Intro_LateUpdate();
void CPZ1Intro_Draw();
void CPZ1Intro_Setup(void* subtype);
void CPZ1Intro_StageLoad();
void CPZ1Intro_GetAttributes();

#endif //!OBJ_CPZ1INTRO_H
