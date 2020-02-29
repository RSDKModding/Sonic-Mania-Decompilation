#ifndef OBJ_PSZEGGMAN_H
#define OBJ_PSZEGGMAN_H

#include "../../SonicMania.h"

//Object Class
class ObjectPSZEggman : public Object {
public:

};

//Entity Class
class EntityPSZEggman : public Entity {
public:

};

//Object Entity
ObjectPSZEggman PSZEggman;

//Entity Functions
void PSZEggman_Update();
void PSZEggman_EarlyUpdate();
void PSZEggman_LateUpdate();
void PSZEggman_Draw();
void PSZEggman_Setup(void* subtype);
void PSZEggman_StageLoad();
void PSZEggman_GetAttributes();

#endif //!OBJ_PSZEGGMAN_H
