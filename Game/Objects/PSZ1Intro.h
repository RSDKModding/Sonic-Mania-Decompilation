#ifndef OBJ_PSZ1INTRO_H
#define OBJ_PSZ1INTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectPSZ1Intro : public Object {
public:

};

//Entity Class
class EntityPSZ1Intro : public Entity {
public:

};

//Object Entity
ObjectPSZ1Intro PSZ1Intro;

//Entity Functions
void PSZ1Intro_Update();
void PSZ1Intro_EarlyUpdate();
void PSZ1Intro_LateUpdate();
void PSZ1Intro_Draw();
void PSZ1Intro_Setup(void* subtype);
void PSZ1Intro_StageLoad();
void PSZ1Intro_GetAttributes();

#endif //!OBJ_PSZ1INTRO_H
