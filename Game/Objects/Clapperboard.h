#ifndef OBJ_CLAPPERBOARD_H
#define OBJ_CLAPPERBOARD_H

#include "../../SonicMania.h"

//Object Class
class ObjectClapperboard : public Object {
public:

};

//Entity Class
class EntityClapperboard : public Entity {
public:

};

//Object Entity
ObjectClapperboard Clapperboard;

//Entity Functions
void Clapperboard_Update();
void Clapperboard_EarlyUpdate();
void Clapperboard_LateUpdate();
void Clapperboard_Draw();
void Clapperboard_Setup(void* subtype);
void Clapperboard_StageLoad();
void Clapperboard_GetAttributes();

#endif //!OBJ_CLAPPERBOARD_H
