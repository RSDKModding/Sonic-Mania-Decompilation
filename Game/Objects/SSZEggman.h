#ifndef OBJ_SSZEGGMAN_H
#define OBJ_SSZEGGMAN_H

#include "../../SonicMania.h"

//Object Class
class ObjectSSZEggman : public Object {
public:

};

//Entity Class
class EntitySSZEggman : public Entity {
public:

};

//Object Entity
ObjectSSZEggman SSZEggman;

//Entity Functions
void SSZEggman_Update();
void SSZEggman_EarlyUpdate();
void SSZEggman_LateUpdate();
void SSZEggman_Draw();
void SSZEggman_Setup(void* subtype);
void SSZEggman_StageLoad();
void SSZEggman_GetAttributes();

#endif //!OBJ_SSZEGGMAN_H
