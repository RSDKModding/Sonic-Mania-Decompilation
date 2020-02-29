#ifndef OBJ_PSZ2INTRO_H
#define OBJ_PSZ2INTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectPSZ2Intro : public Object {
public:

};

//Entity Class
class EntityPSZ2Intro : public Entity {
public:

};

//Object Entity
ObjectPSZ2Intro PSZ2Intro;

//Entity Functions
void PSZ2Intro_Update();
void PSZ2Intro_EarlyUpdate();
void PSZ2Intro_LateUpdate();
void PSZ2Intro_Draw();
void PSZ2Intro_Setup(void* subtype);
void PSZ2Intro_StageLoad();
void PSZ2Intro_GetAttributes();

#endif //!OBJ_PSZ2INTRO_H
