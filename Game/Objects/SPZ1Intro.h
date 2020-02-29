#ifndef OBJ_SPZ1INTRO_H
#define OBJ_SPZ1INTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectSPZ1Intro : public Object {
public:

};

//Entity Class
class EntitySPZ1Intro : public Entity {
public:

};

//Object Entity
ObjectSPZ1Intro SPZ1Intro;

//Entity Functions
void SPZ1Intro_Update();
void SPZ1Intro_EarlyUpdate();
void SPZ1Intro_LateUpdate();
void SPZ1Intro_Draw();
void SPZ1Intro_Setup(void* subtype);
void SPZ1Intro_StageLoad();
void SPZ1Intro_GetAttributes();

#endif //!OBJ_SPZ1INTRO_H
