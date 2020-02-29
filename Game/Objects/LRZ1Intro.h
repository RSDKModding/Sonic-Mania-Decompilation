#ifndef OBJ_LRZ1INTRO_H
#define OBJ_LRZ1INTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZ1Intro : public Object {
public:

};

//Entity Class
class EntityLRZ1Intro : public Entity {
public:

};

//Object Entity
ObjectLRZ1Intro LRZ1Intro;

//Entity Functions
void LRZ1Intro_Update();
void LRZ1Intro_EarlyUpdate();
void LRZ1Intro_LateUpdate();
void LRZ1Intro_Draw();
void LRZ1Intro_Setup(void* subtype);
void LRZ1Intro_StageLoad();
void LRZ1Intro_GetAttributes();

#endif //!OBJ_LRZ1INTRO_H
