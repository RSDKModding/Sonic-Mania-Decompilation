#ifndef OBJ_HCZ1INTRO_H
#define OBJ_HCZ1INTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectHCZ1Intro : public Object {
public:

};

//Entity Class
class EntityHCZ1Intro : public Entity {
public:

};

//Object Entity
ObjectHCZ1Intro HCZ1Intro;

//Entity Functions
void HCZ1Intro_Update();
void HCZ1Intro_EarlyUpdate();
void HCZ1Intro_LateUpdate();
void HCZ1Intro_Draw();
void HCZ1Intro_Setup(void* subtype);
void HCZ1Intro_StageLoad();
void HCZ1Intro_GetAttributes();

#endif //!OBJ_HCZ1INTRO_H
