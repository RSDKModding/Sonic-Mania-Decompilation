#ifndef OBJ_TURBOSPIKER_H
#define OBJ_TURBOSPIKER_H

#include "../../SonicMania.h"

//Object Class
class ObjectTurboSpiker : public Object {
public:

};

//Entity Class
class EntityTurboSpiker : public Entity {
public:

};

//Object Entity
ObjectTurboSpiker TurboSpiker;

//Entity Functions
void TurboSpiker_Update();
void TurboSpiker_EarlyUpdate();
void TurboSpiker_LateUpdate();
void TurboSpiker_Draw();
void TurboSpiker_Setup(void* subtype);
void TurboSpiker_StageLoad();
void TurboSpiker_GetAttributes();

#endif //!OBJ_TURBOSPIKER_H
