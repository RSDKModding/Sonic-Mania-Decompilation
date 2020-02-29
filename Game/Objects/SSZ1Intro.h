#ifndef OBJ_SSZ1INTRO_H
#define OBJ_SSZ1INTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectSSZ1Intro : public Object {
public:

};

//Entity Class
class EntitySSZ1Intro : public Entity {
public:

};

//Object Entity
ObjectSSZ1Intro SSZ1Intro;

//Entity Functions
void SSZ1Intro_Update();
void SSZ1Intro_EarlyUpdate();
void SSZ1Intro_LateUpdate();
void SSZ1Intro_Draw();
void SSZ1Intro_Setup(void* subtype);
void SSZ1Intro_StageLoad();
void SSZ1Intro_GetAttributes();

#endif //!OBJ_SSZ1INTRO_H
